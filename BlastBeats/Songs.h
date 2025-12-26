#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
#include <mutex>
#include "MusicDirectories.h"

namespace Songs {
	class Song {
	private:
		std::shared_ptr<MusicDirectories::MusicDirectory> m_SourceDir;
		std::wstring m_Filepath{};
		std::wstring m_Title{};
		std::wstring m_Artist{};
		std::wstring m_Genre{};
	public:
		Song() {};
		Song(std::wstring filepath, std::wstring title, std::wstring artist, std::wstring  genre) {
			m_Filepath = filepath;
			m_Title = title;
			m_Artist = artist;
			m_Genre = genre;
		}

		std::wstring GetTitle();
		std::wstring GetArtist();
		std::wstring GetGenre();

		bool IsEmpty();
	};

	class SongList {
	private:
		std::vector<std::shared_ptr<Song>> Songs{};
		mutable std::mutex Mutex;
	public:
		SongList(size_t reserveSize);
		void AddRange(const std::vector<std::shared_ptr<Song>>& songsToAdd);
		std::vector<std::shared_ptr<Song>> GetSongs();
	};

	std::vector<std::shared_ptr<Song>> LoadAllSongsInDirectory(const std::filesystem::path& dirPath);
	std::shared_ptr<SongList> LoadAllSongsInDirectory(const std::shared_ptr<MusicDirectories::MusicDirectory> musicDir);
}