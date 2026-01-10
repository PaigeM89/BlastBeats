#pragma once
#include "MusicPlayerGui.h"
#include <AppState.h>
#include "imgui.h"

void MusicPlayerGui::Render(std::shared_ptr<AppState::ApplicationState> state, std::shared_ptr<Helpers::Callbacks> callbacks)
{
	const auto& currentSong = state->GetCurrentSong();
	if (currentSong)
	{
		ImGui::Begin("Now Playing");

		ImGui::LabelText("Song", callbacks->WCharToUtf8(currentSong->GetTitle()).c_str());
		ImGui::LabelText("Artist", callbacks->WCharToUtf8(currentSong->GetArtist()).c_str());
		ImGui::LabelText("Album", callbacks->WCharToUtf8(currentSong->GetAlbum()).c_str());

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Now Playing");

		ImGui::LabelText("Song", "Select a song!");

		ImGui::End();
	}
}