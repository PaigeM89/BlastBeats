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
		uuids::uuid Id;
		std::wstring DirPath;
		bool FlaggedForRemoval;

		MusicDirectory(std::wstring dirPath) {
			Id = uuids::uuid_system_generator{}();
			DirPath = dirPath;
			FlaggedForRemoval = false;
		}
	};

	/// <summary>
	/// The data required to launch a thread to load songs from a specific directory.
	/// </summary>
	struct SongLoadingThreadData
	{
		std::shared_ptr<Songs::SongList> m_SongManager;
		std::shared_ptr<MusicDirectories::MusicDirectory> m_MusicDir;
		bool m_IsCompleted = false;

		SongLoadingThreadData(std::shared_ptr<Songs::SongList> songManager, std::shared_ptr<MusicDirectories::MusicDirectory> musicDir)
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
		SongLoadingThreadManager(std::shared_ptr<Songs::SongList> songManager, std::shared_ptr<MusicDirectories::MusicDirectory> musicDir) : _data(songManager, musicDir) {
			wprintf(L"BLASTBEATS: Loading data in dir %s\n", _data.m_MusicDir->DirPath.c_str());
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
			return _data.m_MusicDir->DirPath;
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
		std::shared_ptr<Songs::SongList> m_SongManager;

		void ReadSongsInDirectory(std::shared_ptr<MusicDirectory> dir);
		void RemoveSongsInDirectory(const std::wstring& dirPath);
		void RemoveSongsInDirectory(const uuids::uuid& musicDirId);
		uuids::uuid GetMusicDirId(const std::wstring& dirPath);
	public:
		MusicDirectoryManager();
		void AddDirectory(std::wstring& dirPath);
		void RemoveDirectory(std::wstring& directory);
		void RemoveDirectory(uuids::uuid musicDirId);
		void Update(std::shared_ptr<Messages::MusicDirectoryChanged> msg) override;

		bool IsLoadingSongs();
		void CheckForCompletedThreads();
		size_t GetSongCount();

		~MusicDirectoryManager();
	};

	std::shared_ptr<MusicDirectory> CreateMusicDirectory(std::wstring dirPath);
}
