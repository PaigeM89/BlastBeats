#pragma once
#include "external/imgui/imgui.h"
#include "SongsTable.h"
#include <Songs.h>
#include <AppState.h>
#include "WindowsHelpers.h"


void SongsTable::Render(std::shared_ptr<AppState::ApplicationState> state)
{
	if (ImGui::BeginTable("songsTable", 4))
	{
		ImGui::TableNextColumn();
		ImGui::Text("Title");
		ImGui::TableNextColumn();
		ImGui::Text("Album");
		ImGui::TableNextColumn();
		ImGui::Text("Artist");
		ImGui::TableNextColumn();
		ImGui::Text("Genre");
		for (const auto& song : state->GetSongs())
		{
			ImGui::TableNextColumn();
			ImGui::Text(Helpers::WCharToUtf8(song->GetTitle()).c_str());
			ImGui::TableNextColumn();
			ImGui::Text(Helpers::WCharToUtf8(song->GetAlbum()).c_str());
			ImGui::TableNextColumn();
			ImGui::Text(Helpers::WCharToUtf8(song->GetArtist()).c_str());
			ImGui::TableNextColumn();
			ImGui::Text(Helpers::WCharToUtf8(song->GetGenre()).c_str());
		}
		ImGui::EndTable();
	}
	return;
}


