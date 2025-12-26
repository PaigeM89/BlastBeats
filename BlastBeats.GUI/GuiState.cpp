#pragma once
#include <vector>
#include "GuiState.h"

std::vector<std::shared_ptr<GuiState::MusicDirectoryGuiState>> GuiState::GuiState::GetMusicDirectories()
{
	return m_MusicDirectories;
}