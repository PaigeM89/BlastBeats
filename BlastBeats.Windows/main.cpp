#pragma once
#include <stdio.h>
#include <AppState.h>
#include <WindowsCallbacks.h>

int main(int, char**) {
    printf("BLASTBEATS: Hello from BlastBeats!\n");
    const auto& appState = AppState::ApplicationState();
    const auto& callbacks = WindowsHelpers::CreateCallbacks();
    return 0;
}

