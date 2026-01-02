#pragma once
#include <vector>
#include "GuiState.h"

std::vector<std::shared_ptr<GuiState::MusicDirectoryState>> GuiState::GuiState::GetMusicDirectories()
{
	return m_MusicDirectories;
}

void GuiState::MusicDirectoryState::SetDirPath(const std::wstring& dirPath)
{
	this->m_DirPath = dirPath;
	auto msg = std::make_shared<Messages::MusicDirectoryChanged>();
	msg->Id = this->Id;	
	msg->DirPath = this->m_DirPath;
	Notify(msg);
}

void GuiState::MusicDirectoryState::SetFlaggedForRemoval()
{
	this->m_FlaggedForRemoval = true;
	auto msg = std::make_shared<Messages::MusicDirectoryChanged>();
	msg->Id = this->Id;
	msg->DirPath = this->m_DirPath;
	msg->FlaggedForRemoval = true;
	Notify(msg);
}