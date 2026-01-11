#pragma once
#include <stdio.h>
#include <AppState.h>
#include <WindowsCallbacks.h>
#include <MainGui.h>

int main(int, char**) {
    printf("BLASTBEATS: Hello from BlastBeats!\n");
    const auto& appState = std::make_shared<AppState::ApplicationState>();
    const auto& callbacks = WindowsHelpers::CreateCallbacks();

    MainGui::Render(appState, callbacks);

    return 0;
}

