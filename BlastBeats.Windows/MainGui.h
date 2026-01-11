#pragma once
#include <memory>
#include <AppState.h>
#include <Helpers.h>

namespace MainGui
{
    void Render(std::shared_ptr<AppState::ApplicationState> appstate, std::shared_ptr<Helpers::Callbacks> callbacks);
} // namespace MainGui


