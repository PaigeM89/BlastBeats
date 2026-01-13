#include "Songs.h"
#include <SFML/Audio.hpp>
#include <filesystem>
#include <taglib/fileref.h>
// #include <fileref.h>
// #include <fileref.h>
// #include <tag/fileref.h>
// #include <fileref.h>
// #include "fileref.h"
// #include "fileref.h"

#include <iostream>
#include <thread>

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
	//TagLib::FileRef fileref(filepath.c_str());
	//std::shared_ptr<TagLib::FileRef> fileref = std::make_shared<TagLib::FileRef>(filepath.c_str());
	/*if (!fileref.isNull() && fileref.tag())
	{*/
	const auto& song = std::make_shared<Songs::Song>(musicDirId, filepath);
		//fileref.~FileRef(); // do i need to do this? will it auto-delete when it goes out of scope?
	return song;
		//auto title = fileref.tag()->title().toWString();
		//auto album = fileref.tag()->album().toWString();
		//auto artist = fileref.tag()->artist().toWString();
		//auto genre = fileref.tag()->genre().toWString();
		//return std::make_shared<Songs::Song>(musicDirId, filepath, title, album, artist, genre);
	//}
	//fileref.~FileRef();
	//return std::make_shared<Songs::Song>();
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

//Songs::Song::Song(const uuids::uuid& musicDirId, const std::wstring& filepath, const TagLib::FileRef& fileref) : Songs::Song::Song(musicDirId, filepath)
//{
//	const auto title = fileref.tag()->title();
//	std::cout << "title is: " << title << std::endl;
//	auto test = std::wstring(title.toCWString());
//	std::cout << "test title is: " << test << std::endl;
//	m_Title = title.toWString();
//	m_Album = fileref.tag()->album().toWString();
//	m_Artist = L"Test";// fileref.tag()->artist().toWString();
//	m_Genre = fileref.tag()->genre().toWString();
//	m_PlayTimeInSeconds = fileref.audioProperties()->lengthInSeconds();
//	std::cout << "finished loading song tags" << std::endl;
//}

Songs::Song::Song(const uuids::uuid& musicDirId, const std::wstring& filePath)
{
	m_MusicDirId = musicDirId;
	m_Filepath = filePath;
	const auto& fp = filePath.c_str();

	std::wcout << "filepath: " << fp << std::endl;

	TagLib::FileRef fileref(fp);
	if (!fileref.isNull() && fileref.tag())
	{
		const auto title = fileref.tag()->title();
		std::cout << "title is: " << title << std::endl;
		auto test = std::wstring(title.toCWString());
		std::cout << "test title is: " << test << std::endl;
		m_Title = title.toWString();
		m_Album = fileref.tag()->album().toWString();
		m_Artist = L"Test";// fileref.tag()->artist().toWString();
		m_Genre = fileref.tag()->genre().toWString();
		m_PlayTimeInSeconds = fileref.audioProperties()->lengthInSeconds();
		std::cout << "finished loading song tags" << std::endl;
	}
	fileref.~FileRef();
}

//Songs::Song::Song(const uuids::uuid& musicDirId, const std::wstring& filepath, const std::shared_ptr<TagLib::FileRef> fileref) : Songs::Song::Song(musicDirId, filepath)
//{
//	const auto& title = fileref->tag()->title().toWString();
//	std::cout << "title is: " << title << std::endl;
//	m_Title = fileref->tag()->title().toWString();
//	
//	const auto& album = fileref->tag()->album().toWString();
//	std::cout << "album is: " << album << std::endl;
//	m_Album = fileref->tag()->album().toWString();
//	
//	const auto& artist = fileref->tag()->artist().toWString();
//	std::cout << "artist is: " << artist << std::endl;
//	m_Artist = artist; // fileref->tag()->artist().toWString();
//	
//	const auto& genre = fileref->tag()->genre().toWString();
//	std::cout << "genre is: " << genre << std::endl;
//	m_Genre = fileref->tag()->genre().toWString();
//	m_PlayTimeInSeconds = fileref->audioProperties()->lengthInSeconds();
//}

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

void Songs::SongManager::AddLoadingDir(const uuids::uuid dirId)
{
	std::lock_guard<std::mutex> loadingDirsLock(m_LoadingDirsMutex);
	m_LoadingDirIds.push_back(dirId);
}

void Songs::SongManager::RemoveLoadingDir(const uuids::uuid dirId)
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