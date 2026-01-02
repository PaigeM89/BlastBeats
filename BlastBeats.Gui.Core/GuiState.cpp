#pragma once
#include <vector>
#include "GuiState.h"

std::vector<std::shared_ptr<GuiState::MusicDirectoryState>> GuiState::RootGuiState::GetMusicDirectories()
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

std::wstring GuiState::MusicDirectoryState::GetDirPath()
{
	return this->m_DirPath;
}

GuiState::MusicDirectoryState::MusicDirectoryState(std::wstring& dirPath)
{
	Id = uuids::uuid_system_generator{}();
	m_DirPath = dirPath;
	auto msg = std::make_shared<Messages::MusicDirectoryChanged>();
	msg->Id = this->Id;
	msg->DirPath = this->m_DirPath;
	Notify(msg);
}

void GuiState::RootGuiState::AddMusicDirectory(std::wstring& dirPath)
{
	m_MusicDirectories.emplace_back(std::make_shared<GuiState::MusicDirectoryState>(dirPath));
}