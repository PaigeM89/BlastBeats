#pragma once
#include <memory>
#include <AppState.h>
#include <Helpers.h>

namespace MusicPlayerGui {

	void Render(std::shared_ptr<AppState::ApplicationState> state, std::shared_ptr<Helpers::Callbacks> callbacks);

}