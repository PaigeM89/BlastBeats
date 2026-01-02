#pragma once
#include <memory>
#include "AppState.h"

namespace MainGui
{
	int RunGui(std::shared_ptr<AppState::ApplicationState> appState);
}
