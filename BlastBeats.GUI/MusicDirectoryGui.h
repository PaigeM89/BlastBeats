#pragma once
#include <memory>
#include "AppState.h"

namespace MusicDirectoryGui
{
	void Render(std::shared_ptr<AppState::ApplicationState> appState);
}