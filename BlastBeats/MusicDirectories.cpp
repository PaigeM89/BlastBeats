#include "MusicDirectories.h"
#include <memory>

std::shared_ptr<MusicDirectories::MusicDirectory> MusicDirectories::CreateMusicDirectory(std::wstring dirPath)
{
    return std::make_shared<MusicDirectories::MusicDirectory>(dirPath);
}


void MusicDirectories::MusicDirectoryManager::AddDirectory(std::wstring& dirPath)
{
	m_MusicDirectories.emplace_back(MusicDirectories::CreateMusicDirectory(dirPath));
}

void MusicDirectories::MusicDirectoryManager::RemoveDirectory(std::wstring& directory)
{
	size_t index;
	bool wasSet = false;
	for (size_t i = 0; i < m_MusicDirectories.size(); i++)
	{
		if (m_MusicDirectories[i]->DirPath == directory)
		{
			index = i;
			wasSet = true;
			break;
		}
	}
	if (wasSet)
	{
		RemoveDirectory(index);
	}
}

void MusicDirectories::MusicDirectoryManager::RemoveDirectory(size_t index)
{
	if (m_MusicDirectories.size() > index && index > 0)
	{
		m_MusicDirectories.erase(m_MusicDirectories.begin() + index);
	}
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
				return;
			}
		}
	}
}

