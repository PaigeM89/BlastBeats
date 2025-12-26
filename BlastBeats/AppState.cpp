#pragma once
#include "AppState.h"
#include <memory>

using namespace AppState;

std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> ApplicationState::GetMusicDirectories()
{
	return this->m_MusicDirectories;
}

void ApplicationState::AddMusicDirectory(std::wstring dirPath)
{
	/*auto msd = MusicDirectories::CreateMusicDirectory(dirPath);
	AddMusicDirectory(msd);*/
	m_MusicDirectories.emplace_back(MusicDirectories::CreateMusicDirectory(dirPath));
}

void ApplicationState::RemoveDirectory(std::wstring directory)
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

void ApplicationState::RemoveDirectory(size_t index)
{
	if (m_MusicDirectories.size() > index && index > 0)
	{
		m_MusicDirectories.erase(m_MusicDirectories.begin() + index);
	}
}
