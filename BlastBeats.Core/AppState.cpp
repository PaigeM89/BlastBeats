#pragma once
#include "AppState.h"
#include <memory>

using namespace AppState;

AppState::ApplicationState::ApplicationState(std::shared_ptr<Helpers::Callbacks> callbacks)
{
	m_Config = std::make_shared<AppConfig::Config>(callbacks);
	m_MusicDirectoryManager = std::make_shared<MusicDirectories::MusicDirectoryManager>(m_Config, callbacks);
	m_Player = std::make_shared<MusicPlayer::Player>();
	m_Queue = std::make_unique<SongQueue::Queue>();
	m_Callbacks = callbacks;
}

std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> ApplicationState::GetMusicDirectories()
{
	return m_MusicDirectoryManager->GetMusicDirectories();
}

void ApplicationState::AddMusicDirectory(const std::wstring& dirPath)
{
	m_MusicDirectoryManager->AddDirectory(dirPath);
}

void ApplicationState::RemoveDirectory(const std::wstring& directory)
{
	m_MusicDirectoryManager->RemoveDirectory(directory);
}

void AppState::ApplicationState::PlaySong(std::shared_ptr<Songs::Song> song)
{
	m_Player->SetAndPlaySong(song);
}

bool AppState::ApplicationState::IsSongPlaying()
{
	return m_Player->IsSongPlaying();
}

void AppState::ApplicationState::PauseCurrentSong()
{
	m_Player->Stop();
}

std::shared_ptr<Songs::Song> AppState::ApplicationState::GetCurrentSong()
{
	return m_Player->GetCurrentSong();
}

std::optional<std::shared_ptr<Songs::Song>> AppState::ApplicationState::NextSongInQueue()
{
	return m_Queue->GetNextSong();
}

std::optional<std::shared_ptr<Songs::Song>> AppState::ApplicationState::FirstSongInQueue()
{
	return m_Queue->GetFirstSong();
}

AppState::ApplicationState::~ApplicationState()
{
	m_Queue.release();
}

std::vector<std::shared_ptr<Songs::Song>> AppState::ApplicationState::GetSongs()
{
	return m_MusicDirectoryManager->GetSongs();
}

std::vector<std::shared_ptr<Songs::Song>> AppState::ApplicationState::GetQueuedSongs()
{
	return m_Queue->GetQueue();
}

std::vector<std::shared_ptr<Songs::Song>> AppState::ApplicationState::GetSelectedSongs()
{
	const auto& songs = m_MusicDirectoryManager->GetSongs();
	std::vector<std::shared_ptr<Songs::Song>> selectedSongs{};
	for (const auto& song : songs)
	{
		if (song->p_IsSelected)
			selectedSongs.push_back(song);
	}
	return selectedSongs;
}

void AppState::ApplicationState::AddSongsToEndOfQueue(const std::vector<std::shared_ptr<Songs::Song>>& songs)
{
	m_Queue->AddSongsToEnd(songs);
	m_MusicDirectoryManager->UnselectAllSongs();
}
