#pragma once
#include <string>
#include <memory>
#include <mutex>
#include "Uuid.h"
#include "Observers.h"
#include "Messages.h"
#include "Songs.h"
#include <future>

namespace MusicDirectories {

	/// <summary>
	/// Basic data regarding a directory we should use to load music.
	/// </summary>
	struct MusicDirectory {
		/// <summary>
		/// Randomly generated UUID to more easily track this specific directory.
		/// TODO: Do i need this any more? Unsure.
		/// </summary>
		uuids::uuid m_Id;
		/// <summary>
		/// The path to the directory to load music from.
		/// </summary>
		std::wstring m_DirPath;
		/// <summary>
		/// Used to determine if this directory and all music in it should be removed from the loaded data.
		/// This does NOT flag the actual data on disk for deletion!
		/// </summary>
		bool m_FlaggedForRemoval = false;
		/// <summary>
		/// When TRUE, the user has this directory selected in the list box.
		/// </summary>
		bool m_IsSelected = false;

		MusicDirectory(std::wstring dirPath) {
			m_Id = uuids::uuid_system_generator{}();
			m_DirPath = dirPath;
		}
	};

	/// <summary>
	/// The data required to launch a thread to load songs from a specific directory.
	/// </summary>
	struct SongLoadingThreadData
	{
		std::shared_ptr<Songs::SongManager> m_SongManager;
		std::shared_ptr<MusicDirectories::MusicDirectory> m_MusicDir;
		bool m_IsCompleted = false;

		SongLoadingThreadData(std::shared_ptr<Songs::SongManager> songManager, std::shared_ptr<MusicDirectories::MusicDirectory> musicDir)
		{
			m_SongManager = songManager;
			m_MusicDir = musicDir;
			m_IsCompleted = false; // shouldn't need to do this, doubt it will fix anything
		}
	};

	/// <summary>
	/// Reads song data with a jthread, updating the data in the received `threadData` object.
	/// </summary>
	/// <param name="stopToken"></param>
	/// <param name="threadData"></param>
	void ReadSongsIntoSongManager(std::stop_token stopToken, SongLoadingThreadData threadData);

	/// <summary>
	/// Controls the thread used to load song data from the specified directory.
	/// </summary>
	struct SongLoadingThreadManager {
		SongLoadingThreadManager(std::shared_ptr<Songs::SongManager> songManager, std::shared_ptr<MusicDirectories::MusicDirectory> musicDir) : _data(songManager, musicDir) {
			wprintf(L"BLASTBEATS: Loading data in dir %s\n", _data.m_MusicDir->m_DirPath.c_str());
			_thread = std::jthread(ReadSongsIntoSongManager, _data);
		}

		~SongLoadingThreadManager() {
			_thread.request_stop();
		}

		bool IsCompleted() const
		{
			return _data.m_IsCompleted;
		}

		std::wstring GetMusicDirPath()
		{
			return _data.m_MusicDir->m_DirPath;
		}

		// delete these functions, according to AI
		
		// Explicitly delete copy operations
		SongLoadingThreadManager(const SongLoadingThreadManager&) = delete;
		SongLoadingThreadManager& operator=(const SongLoadingThreadManager&) = delete;

		// Use default move operations
		SongLoadingThreadManager(SongLoadingThreadManager&&) = default;
		SongLoadingThreadManager& operator=(SongLoadingThreadManager&&) = default;
	private:
		std::jthread _thread;
		SongLoadingThreadData _data;
	};

	/// <summary>
	/// Manages the directories used to load music, as well as the master list of song data.
	/// </summary>
	class MusicDirectoryManager : public Observers::MusicDirectoryObserver
	{
	private:
		std::vector<SongLoadingThreadManager> m_SongLoadingThreads{};
		std::vector<std::shared_ptr<MusicDirectory>> m_MusicDirectories{};
		std::shared_ptr<Songs::SongManager> m_SongManager;

		void ReadSongsInDirectory(std::shared_ptr<MusicDirectory> dir);
		void RemoveSongsInDirectory(const std::wstring& dirPath);
		void RemoveSongsInDirectory(const uuids::uuid& musicDirId);
		uuids::uuid GetMusicDirId(const std::wstring& dirPath);
	public:
		MusicDirectoryManager();
		void AddDirectory(const std::wstring& dirPath);
		void RemoveDirectory(const std::wstring& directory);
		void RemoveDirectory(const uuids::uuid musicDirId);
		void Update(std::shared_ptr<Messages::MusicDirectoryChanged> msg) override;

		bool IsLoadingSongs();
		void CheckForCompletedThreads();
		size_t GetSongCount();
		std::vector<std::shared_ptr<MusicDirectory>> GetMusicDirectories();
		std::vector<std::shared_ptr<Songs::Song>> GetSongs();

		~MusicDirectoryManager();
	};

	std::shared_ptr<MusicDirectory> CreateMusicDirectory(std::wstring dirPath);
}
