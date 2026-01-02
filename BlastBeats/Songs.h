#pragma once
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
#include <mutex>
#include "Uuid.h"

namespace Songs {

	/// <summary>
	/// General metadata regarding a single Song.
	/// </summary>
	class Song {
	private:
		uuids::uuid m_MusicDirId;
		std::wstring m_Filepath{};
		std::wstring m_Title{};
		std::wstring m_Artist{};
		std::wstring m_Genre{};
	public:
		Song() {};
		Song(uuids::uuid musicDirId, std::wstring filepath, std::wstring title, std::wstring artist, std::wstring  genre) {
			m_MusicDirId = musicDirId;
			m_Filepath = filepath;
			m_Title = title;
			m_Artist = artist;
			m_Genre = genre;
		}

		uuids::uuid GetMusicDirectoryId();
		std::wstring GetTitle();
		std::wstring GetArtist();
		std::wstring GetGenre();

		bool IsEmpty();
	};

	/// <summary>
	/// Thread-safe container to load songs and keep them in a single master list.
	/// </summary>
	class SongManager {
	private:
		std::vector<std::shared_ptr<Song>> m_Songs{};
		mutable std::mutex m_Mutex;

		mutable std::mutex m_LoadingDirsMutex;
		std::vector<uuids::uuid> m_LoadingDirIds{};

		void AddLoadingDir(const uuids::uuid dirId);
		void RemoveLoadingDir(const uuids::uuid dirId);
		void LoadSongs(const uuids::uuid& musicDirId, const std::vector<std::wstring>& songPaths);
	public:
		SongManager(size_t reserveSize);
		void AddRange(const std::vector<std::shared_ptr<Song>>& songsToAdd);
		std::vector<std::shared_ptr<Song>> GetSongs();

		void LoadSongs(std::stop_token stopToken, const uuids::uuid& musicDirId, const std::vector<std::wstring>& songPaths);

		void RemoveSongsWithDirectoryId(const uuids::uuid& musicDirId);
		bool IsLoadingSongs();
	};

	std::vector<std::wstring> GetAllFilepaths(const std::filesystem::path& rootDirPath);
}