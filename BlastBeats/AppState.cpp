#pragma once
#include "AppState.h"
#include <memory>

using namespace AppState;

AppState::ApplicationState::ApplicationState()
{
	m_MusicDirectoryManager = std::make_shared<MusicDirectories::MusicDirectoryManager>();
	m_Player = std::make_shared<MusicPlayer::Player>();
}

std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> ApplicationState::GetMusicDirectories()
{
	return m_MusicDirectoryManager->GetMusicDirectories();
}

void ApplicationState::AddMusicDirectory(const std::wstring& dirPath)
{
	m_MusicDirectoryManager->AddDirectory(dirPath);
	//m_MusicDirectories.emplace_back(MusicDirectories::CreateMusicDirectory(dirPath));
}

void ApplicationState::RemoveDirectory(const std::wstring& directory)
{
	m_MusicDirectoryManager->RemoveDirectoryW(directory);
}

void AppState::ApplicationState::PlaySong(std::shared_ptr<Songs::Song> song)
{
	m_Player->PlaySong(song);
}

std::vector<std::shared_ptr<Songs::Song>> AppState::ApplicationState::GetSongs()
{
	return m_MusicDirectoryManager->GetSongs();
}
