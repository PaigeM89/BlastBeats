#pragma once
#include <memory>
#include "GuiState.h"

namespace MainGui
{
	int RunGui(std::shared_ptr<GuiState::GuiState> appState);
}
