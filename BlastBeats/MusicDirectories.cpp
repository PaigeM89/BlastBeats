#pragma once
#include "MusicDirectories.h"
#include <memory>
#include <mutex>
#include <filesystem>


void MusicDirectories::ReadSongsIntoSongManager(std::stop_token stopToken, SongLoadingThreadData threadData)
{
	if (threadData.m_IsCompleted)
	{
		wprintf(L"BLASTBEATS: Already completed loading songs for directory %s\n", threadData.m_MusicDir->DirPath.c_str());
		return;
	}
	wprintf(L"BLASTBEATS: Loading songs in directory: %s\n", threadData.m_MusicDir->DirPath.c_str());
	if (!std::filesystem::is_directory(threadData.m_MusicDir->DirPath))
	{
		threadData.m_IsCompleted = true;
		return;
	}

	auto songPaths = Songs::GetAllFilepaths(threadData.m_MusicDir->DirPath);
	threadData.m_SongManager->LoadSongs(stopToken, threadData.m_MusicDir->Id, songPaths);
	threadData.m_IsCompleted = true;
}

std::shared_ptr<MusicDirectories::MusicDirectory> MusicDirectories::CreateMusicDirectory(std::wstring dirPath)
{
	wprintf(L"Adding directory: %s\n", dirPath.c_str());
    return std::make_shared<MusicDirectories::MusicDirectory>(dirPath);
}

void ReadSongsInDirectoryIntoSongList(std::stop_token stopToken, std::shared_ptr<Songs::SongList>& songList, std::shared_ptr<MusicDirectories::MusicDirectory> dir)
{
	wprintf(L"BLASTBEATS: Loading songs in directory: %s\n", dir->DirPath.c_str());
	if (!std::filesystem::is_directory(dir->DirPath))
	{
		return;
	}

	auto songPaths = Songs::GetAllFilepaths(dir->DirPath);
	songList->LoadSongs(stopToken, dir->Id, songPaths);
}

void MusicDirectories::MusicDirectoryManager::ReadSongsInDirectory(std::shared_ptr<MusicDirectory> dir)
{
	wprintf(L"BLASTBEATS: Creating thread to load directory %s\n", dir->DirPath.c_str());

	m_SongLoadingThreads.emplace_back(SongLoadingThreadManager(this->m_SongManager, dir));
}

uuids::uuid MusicDirectories::MusicDirectoryManager::GetMusicDirId(const std::wstring& dirPath)
{
	for (auto& musicDir : m_MusicDirectories)
	{
		if (musicDir->DirPath == dirPath)
			return musicDir->Id;
	}
	// todo: log here
	return uuids::uuid();
}

void MusicDirectories::MusicDirectoryManager::RemoveSongsInDirectory(const std::wstring& dirPath)
{
	const auto musicDirId = GetMusicDirId(dirPath);
	this->RemoveSongsInDirectory(musicDirId);
}

void MusicDirectories::MusicDirectoryManager::RemoveSongsInDirectory(const uuids::uuid& musicDirId)
{
	this->m_SongManager->RemoveSongsWithDirectoryId(musicDirId);
}

MusicDirectories::MusicDirectoryManager::MusicDirectoryManager()
{
	this->m_SongManager = std::make_shared<Songs::SongList>(0);
}

void MusicDirectories::MusicDirectoryManager::AddDirectory(std::wstring& dirPath)
{
	auto musicDir = CreateMusicDirectory(dirPath);
	m_MusicDirectories.push_back(musicDir);
	ReadSongsInDirectory(musicDir);
}

void MusicDirectories::MusicDirectoryManager::RemoveDirectory(std::wstring& directory)
{
	uuids::uuid dirId;
	bool wasSet = false;
	for (const auto& musicDir : m_MusicDirectories)
	{
		if (musicDir->DirPath == directory)
		{
			dirId = musicDir->Id;
			wasSet = true;
		}
	}
	if (wasSet)
	{
		RemoveDirectory(dirId);
	}
}

void MusicDirectories::MusicDirectoryManager::RemoveDirectory(uuids::uuid musicDirId)
{
	this->RemoveSongsInDirectory(musicDirId);
	std::erase_if(m_MusicDirectories, [&musicDirId](const std::shared_ptr<MusicDirectories::MusicDirectory> musicDir) {
			return musicDir->Id == musicDirId;
		});
}

void MusicDirectories::MusicDirectoryManager::Update(std::shared_ptr<Messages::MusicDirectoryChanged> msg)
{
	if (msg->FlaggedForRemoval)
	{
		for (auto& mdir : m_MusicDirectories)
		{
			if (mdir->Id == msg->Id)
			{
				mdir->FlaggedForRemoval = true;
				return;
			}
		}
	}
	else
	{
		for (auto& mdir : m_MusicDirectories)
		{
			if (mdir->Id == msg->Id)
			{
				mdir->DirPath = msg->DirPath;
				// todo: reload new directory, clean old songs as needed
				return;
			}
		}
		this->AddDirectory(msg->DirPath);
	}
}

bool MusicDirectories::MusicDirectoryManager::IsLoadingSongs()
{
	return m_SongManager->IsLoadingSongs();
}

void MusicDirectories::MusicDirectoryManager::CheckForCompletedThreads()
{
	std::erase_if(m_SongLoadingThreads, [](MusicDirectories::SongLoadingThreadManager& sltm) {
			return sltm.IsCompleted();
		});
}

size_t MusicDirectories::MusicDirectoryManager::GetSongCount()
{
	return this->m_SongManager->GetSongs().size();
}

MusicDirectories::MusicDirectoryManager::~MusicDirectoryManager()
{
	this->m_SongLoadingThreads.~vector();
}
