#pragma once
#include <vector>
#include "GuiState.h"

std::vector<std::shared_ptr<GuiState::MusicDirectoryGuiState>> GuiState::GuiState::GetMusicDirectories()
{
	return m_MusicDirectories;
}

void GuiState::MusicDirectoryState::SetDirPath(const std::wstring& dirPath)
{
	this->m_DirPath = dirPath;
	Notify();
}

void GuiState::MusicDirectoryState::Notify()
{
	const auto& msg = std::make_shared<Messages::MusicDirectoryChanged>(0, this->m_DirPath, this->m_FlaggedForRemoval);

	for (const auto& observer : m_Observers)
	{
		observer->Update(msg);
	}
}


