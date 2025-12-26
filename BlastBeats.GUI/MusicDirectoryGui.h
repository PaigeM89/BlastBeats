#pragma once
#include <memory>
#include "GuiState.h"

namespace MusicDirectoryGui
{
	void Render(std::shared_ptr<GuiState::GuiState> guiState);
}