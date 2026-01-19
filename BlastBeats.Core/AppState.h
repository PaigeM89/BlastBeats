#pragma once
#include <memory>
#include <vector>
#include <string>
#include <Helpers.h>
#include <AppConfig.h>
#include "Songs.h"
#include "MusicDirectories.h"
#include "MusicPlayer.h"
#include <SongQueue.h>

namespace AppState
{
	class ApplicationState {
	private:
		std::shared_ptr<AppConfig::Config> m_Config;
		std::shared_ptr<MusicDirectories::MusicDirectoryManager> m_MusicDirectoryManager;
		std::shared_ptr<MusicPlayer::Player> m_Player; // should this be a unique pointer?
		std::unique_ptr<SongQueue::Queue> m_Queue;
		std::shared_ptr<Helpers::Callbacks> m_Callbacks;
	public:
		ApplicationState(std::shared_ptr<Helpers::Callbacks> callbacks);
		std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> GetMusicDirectories();
		std::vector<std::shared_ptr<Songs::Song>> GetSongs();
		std::vector<std::shared_ptr<Songs::Song>> GetQueuedSongs();
		std::vector<std::shared_ptr<Songs::Song>> GetSelectedSongs();
		void AddSongsToEndOfQueue(const std::vector<std::shared_ptr<Songs::Song>>& songs);
		void AddMusicDirectory(const std::wstring& dirPath);
		void RemoveDirectory(const std::wstring& directory);
		void PlaySong(std::shared_ptr<Songs::Song> song);
		bool IsSongPlaying();
		void PauseCurrentSong();
		std::shared_ptr<Songs::Song> GetCurrentSong();
		std::optional<std::shared_ptr<Songs::Song>> NextSongInQueue();
		std::optional<std::shared_ptr<Songs::Song>> FirstSongInQueue();

		~ApplicationState();
	};
}
