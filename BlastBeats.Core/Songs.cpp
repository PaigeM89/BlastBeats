#include "Songs.h"
#include <SFML/Audio.hpp>
#include <filesystem>
#include <taglib/fileref.h>
#include <iostream>
#include <thread>
#include "MusicDirectories.h"

// https://github.com/taglib/taglib/blob/master/examples/tagreader.cpp

namespace fs = std::filesystem;

const size_t CHUNK_SIZE = 1000;
const size_t NUMBER_OF_THREADS = 5;

template<typename T>
std::vector<std::vector<T>> chunkVector(const std::vector<T>& source, size_t chunkSize) {
	std::vector<std::vector<T>> result;
	if (chunkSize == 0) return result;

	// Reserve space to minimize reallocations
	result.reserve((source.size() + chunkSize - 1) / chunkSize);

	auto start = source.begin();
	auto end = source.end();

	while (start != end) {
		// Calculate the end iterator for the current chunk
		auto next = std::distance(start, end) >= chunkSize ? start + chunkSize : end;

		// Create a new vector (copy) using the range constructor
		result.emplace_back(start, next);

		// Move the start iterator to the beginning of the next chunk
		start = next;
	}

	return result;
}

std::vector<std::wstring> Songs::GetAllFilepaths(const std::filesystem::path& rootDirPath)
{
	std::vector<std::wstring> filepaths{};
	for (auto& f : fs::directory_iterator(rootDirPath))
	{
		if (f.is_directory())
		{
			auto pathsInDir = GetAllFilepaths(f.path());
			filepaths.insert(filepaths.end(), pathsInDir.begin(), pathsInDir.end());
			pathsInDir.~vector();
		}
		else
		{
			filepaths.emplace_back(f.path().generic_wstring());
		}
	}
	return filepaths;
}

static std::shared_ptr<Songs::Song> ReadSong(const uuids::uuid musicDirId, const std::wstring& filepath)
{
	const auto& song = std::make_shared<Songs::Song>(musicDirId, filepath);
	return song;
}

static void LoadAllSongs(const uuids::uuid musicDirId, std::shared_ptr<Songs::SongManager> songList, const std::vector<std::wstring>& songPaths)
{
	std::vector<std::shared_ptr<Songs::Song>> songs(songPaths.size());
	for (int i = 0; i < songPaths.size(); i++)
	{
		const auto& song = ReadSong(musicDirId, songPaths[i]);
		if (!song->IsEmpty())
			songs[i] = song;
	}
	const auto& erased = std::erase_if(songs, [](auto& song) { return song->IsEmpty(); });
	songList->AddRange(songs);
}

Songs::Song::Song(const uuids::uuid& musicDirId, const std::wstring& filePath)
{
	m_Filepath = filePath;
	m_MusicDirId = musicDirId;
	TagLib::FileRef fileref(filePath.c_str());
	if (!fileref.isNull() && fileref.tag())
	{
		int track = fileref.tag()->track();
		if (track > 0)
			m_SongNumber = track;

		m_Title = fileref.tag()->title().toWString();
		m_Album = fileref.tag()->album().toWString();
		m_Artist = fileref.tag()->artist().toWString();
		m_Genre = fileref.tag()->genre().toWString();
		m_PlayTimeInSeconds = fileref.audioProperties()->lengthInSeconds();
	}
	fileref.~FileRef();
}

uuids::uuid Songs::Song::GetMusicDirectoryId()
{
	return m_MusicDirId;
}

uuids::uuid Songs::Song::GetId() const
{
	return m_Id;
}

std::wstring Songs::Song::GetFilepath()
{
	return m_Filepath;
}

std::optional<int> Songs::Song::GetSongNumber()
{
	if (m_SongNumber > 0)
		return m_SongNumber;
	return {};
}

std::wstring Songs::Song::GetTitle()
{
	return m_Title;
}

std::wstring Songs::Song::GetAlbum()
{
	return m_Album;
}

std::wstring Songs::Song::GetArtist()
{
	return m_Artist;
}

std::wstring Songs::Song::GetGenre()
{
	return m_Genre;
}

bool Songs::Song::IsEmpty()
{
	return this == nullptr || m_Title.length() == 0 && m_Artist.length() == 0;
}

void Songs::SongManager::AddRange(const std::vector<std::shared_ptr<Song>>& songsToAdd)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Songs.insert(m_Songs.end(), songsToAdd.begin(), songsToAdd.end());
}

std::vector<std::shared_ptr<Songs::Song>> Songs::SongManager::GetSongs()
{
	return m_Songs;
}

void Songs::SongManager::AddLoadingDir(const uuids::uuid& dirId)
{
	std::lock_guard<std::mutex> loadingDirsLock(m_LoadingDirsMutex);
	m_LoadingDirIds.push_back(dirId);
}

void Songs::SongManager::RemoveLoadingDir(const uuids::uuid& dirId)
{
	std::lock_guard<std::mutex> loadingDirsLock(m_LoadingDirsMutex);
	std::erase_if(this->m_LoadingDirIds, [&dirId](const uuids::uuid& id) {
		return id == dirId;
		});
}

void Songs::SongManager::LoadSongs(const uuids::uuid& musicDirId, const std::vector<std::wstring>& songPaths)
{
	std::vector<std::shared_ptr<Songs::Song>> songs(songPaths.size());
	for (int i = 0; i < songPaths.size(); i++)
	{
		const auto& song = ReadSong(musicDirId, songPaths[i]);
		if (!song->IsEmpty())
			songs[i] = song;
	}
	const auto& erased = std::erase_if(songs, [](auto& song) { return song->IsEmpty(); });
	this->AddRange(songs);
}

void Songs::SongManager::LoadSongs(std::stop_token stopToken, const uuids::uuid& musicDirId, const std::vector<std::wstring>& songPaths)
{
	this->AddLoadingDir(musicDirId);
	std::vector<std::shared_ptr<Songs::Song>> songs(songPaths.size());
	auto songPathChunks = chunkVector(songPaths, CHUNK_SIZE);
	auto chunkedChunks = chunkVector(songPathChunks, NUMBER_OF_THREADS);

	// todo: this is not gonna be good - maybe use futures here?

	// for each group of 5 chunks...
	for (const auto& chunk : chunkedChunks)
	{
		if (!stopToken.stop_requested())
		{
			std::array<std::thread, NUMBER_OF_THREADS> threads{};
			// make a thread to load each chunk...
			for (int i = 0; i < NUMBER_OF_THREADS; i++)
			{
				if (chunk.size() > i)
					//threads[i] = std::thread([this](){LoadSongs, musicDirId, chunk[i]);
				{
					auto& thisChunk = chunk[i];
					threads[i] = std::thread([this, &musicDirId, thisChunk]() { this->LoadSongs(musicDirId, thisChunk); });
				}
				else
					threads[i] = std::thread();
			}

			// then wait for the threads to complete
			for (auto& thread : threads)
			{
				if (thread.joinable()) {
					thread.join();
				}
			}
		}
		if (stopToken.stop_requested())
		{
			printf("Stop requested on thread loading files\n");
		}
	}

	this->RemoveLoadingDir(musicDirId);
}

void Songs::SongManager::RemoveSongsWithDirectoryId(const uuids::uuid& musicDirId)
{
	std::erase_if(m_Songs, [&musicDirId](std::shared_ptr<Songs::Song>& song) {
		return song->GetMusicDirectoryId() == musicDirId;
		});
}

Songs::SongManager::SongManager(size_t reserveSize)
{
	m_Songs.reserve(reserveSize);
}

bool Songs::SongManager::IsLoadingSongs()
{
	return this->m_LoadingDirIds.size() > 0;
}