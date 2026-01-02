#pragma once
#include "MusicPlayerGui.h"
#include <AppState.h>
#include "external/imgui/imgui.h"
#include "WindowsHelpers.h"

void MusicPlayerGui::Render(std::shared_ptr<AppState::ApplicationState> state)
{
	const auto& currentSong = state->GetCurrentSong();
	if (currentSong)
	{
		ImGui::Begin("Now Playing");

		ImGui::LabelText("Song", Helpers::WCharToUtf8(currentSong->GetTitle()).c_str());
		ImGui::LabelText("Artist", Helpers::WCharToUtf8(currentSong->GetArtist()).c_str());
		ImGui::LabelText("Album", Helpers::WCharToUtf8(currentSong->GetAlbum()).c_str());

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Now Playing");

		ImGui::LabelText("Song", "Select a song!");

		ImGui::End();
	}
}