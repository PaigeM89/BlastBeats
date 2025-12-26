#include "Songs.h"
#include <SFML/Audio.hpp>
#include <filesystem>
#include <taglib/fileref.h>
#include <iostream>
#include <thread>

// https://github.com/taglib/taglib/blob/master/examples/tagreader.cpp

// it took 60 seconds to load everything. Hmm.
// actually deleting the fileref reduced that to 20 seconds! yay!

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

static std::vector<std::wstring> GetAllFilepaths(const std::filesystem::path& rootDirPath)
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

static std::shared_ptr<Songs::Song> ReadSong(const std::wstring& filepath)
{
	TagLib::FileRef fileref(filepath.c_str());
	if (!fileref.isNull() && fileref.tag())
	{
		auto title = fileref.tag()->title().toWString();
		auto artist = fileref.tag()->artist().toWString();
		auto genre = fileref.tag()->genre().toWString();
		return std::make_shared<Songs::Song>(filepath, title, artist, genre);
	}
	return std::make_shared<Songs::Song>();
}

static void LoadAllSongs(std::shared_ptr<Songs::SongList> songList, const std::vector<std::wstring>& songPaths)
{
	std::vector<std::shared_ptr<Songs::Song>> songs(songPaths.size());
	for (int i = 0; i < songPaths.size(); i++)
	{
		const auto& song = ReadSong(songPaths[i]);
		if (!song->IsEmpty())
			songs[i] = song;
	}
	/*for (const auto& songPath : songPaths)
	{
		auto song = ReadSong(songPath);
		if (!song->IsEmpty())
			songs.push_back(song);
	}*/
	const auto& erased = std::erase_if(songs, [](auto& song) { return song->IsEmpty(); });
	songList->AddRange(songs);
}

static void LoadAllSongsParallel(std::shared_ptr<Songs::SongList> songList, const std::vector<std::wstring> songPaths)
{
	std::vector<std::shared_ptr<Songs::Song>> songs(songPaths.size());
	auto songPathChunks = chunkVector(songPaths, CHUNK_SIZE);
	auto chunkedChunks = chunkVector(songPathChunks, NUMBER_OF_THREADS);
	
	// todo: this is not gonna be good
	
	// for each group of 5 chunks...
	for (const auto& chunk : chunkedChunks)
	{
		std::array<std::thread, NUMBER_OF_THREADS> threads{};
		// make a thread to load each chunk...
		for (int i = 0; i < NUMBER_OF_THREADS; i++)
		{
			if (chunk.size() > i)
				threads[i] = std::thread(LoadAllSongs, songList, chunk[i]);
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
}

std::vector<std::shared_ptr<Songs::Song>> Songs::LoadAllSongsInDirectory(const std::filesystem::path& dirPath)
{
	if (!fs::is_directory(dirPath))
	{
		std::vector<std::shared_ptr<Songs::Song>> songs{};
		return songs;
	}
	
	auto songPaths = GetAllFilepaths(dirPath);
	auto sl = std::make_shared<Songs::SongList>(songPaths.size());
	LoadAllSongsParallel(sl, songPaths);

	return sl->GetSongs();
}

std::shared_ptr<Songs::SongList> Songs::LoadAllSongsInDirectory(const std::shared_ptr<MusicDirectories::MusicDirectory> musicDir)
{
	if (!fs::is_directory(musicDir->DirPath) || musicDir->FlaggedForRemoval)
	{
		return std::shared_ptr<Songs::SongList>();
	}
	
	auto songPaths = GetAllFilepaths(musicDir->DirPath);
	auto sl = std::make_shared<Songs::SongList>(songPaths.size());
	LoadAllSongsParallel(sl, songPaths);
	return sl;
}

std::wstring Songs::Song::GetTitle()
{
	return m_Title;
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

void Songs::SongList::AddRange(const std::vector<std::shared_ptr<Song>>& songsToAdd)
{
	std::lock_guard<std::mutex> lock(Mutex);
	Songs.insert(Songs.end(), songsToAdd.begin(), songsToAdd.end());
}

std::vector<std::shared_ptr<Songs::Song>> Songs::SongList::GetSongs()
{
	return Songs;
}

Songs::SongList::SongList(size_t reserveSize)
{
	Songs.reserve(reserveSize);
}