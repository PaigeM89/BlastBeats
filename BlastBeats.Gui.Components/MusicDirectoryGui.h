#pragma once
#include <memory>
#include "AppState.h"
#include <Helpers.h>

namespace MusicDirectoryGui
{
	void Render(std::shared_ptr<AppState::ApplicationState> appState, std::shared_ptr<Helpers::Callbacks> callbacks);
}