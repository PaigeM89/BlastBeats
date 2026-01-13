#pragma once
#include "SongQueueGui.h"
#include <imgui.h>


void SongQueueGui::Render(std::shared_ptr<AppState::ApplicationState> state, std::shared_ptr<Helpers::Callbacks> callbacks)
{
	static ImGuiTableFlags tableFlags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable
		| ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_RowBg;

	static ImGuiSelectableFlags selectable_flags =
		ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;

	static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_None;
	ImGui::Begin("Queued songs");
	if (ImGui::BeginTable("Song Queue", 5, tableFlags, ImVec2(0, 0)))
	{
		ImGui::TableSetupColumn("##SongNumber", column_flags);
		ImGui::TableSetupColumn("Title", column_flags);
		ImGui::TableSetupColumn("Album", column_flags);
		ImGui::TableSetupColumn("Artist", column_flags);
		ImGui::TableSetupColumn("Genre", column_flags);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		for (const auto& song : state->GetQueuedSongs())
		{
			ImGui::PushID(callbacks->WCharToUtf8(song->GetTitle()).c_str());
			const bool isSelected = song->p_IsSelected;
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			if (song->GetSongNumber().has_value())
			{
				const auto& sn = std::to_string(song->GetSongNumber().value());
				ImGui::Text(sn.c_str());
			}
			else
			{
				ImGui::Text("");
			}
			ImGui::TableSetColumnIndex(1);
			// this has to be a value in a column for the row selection to work.
			if (ImGui::Selectable(callbacks->WCharToUtf8(song->GetTitle()).c_str(), isSelected, selectable_flags, ImVec2(0, 0)))
			{
				const auto& title = song->GetTitle();
				wprintf(L"Toggling selection on song %s\n", title.c_str());
				song->p_IsSelected = !isSelected;
			}

			ImGui::TableSetColumnIndex(2);
			ImGui::Text(callbacks->WCharToUtf8(song->GetAlbum()).c_str());
			ImGui::TableSetColumnIndex(3);
			ImGui::Text(callbacks->WCharToUtf8(song->GetArtist()).c_str());
			ImGui::TableSetColumnIndex(4);
			ImGui::Text(callbacks->WCharToUtf8(song->GetGenre()).c_str());

			ImGui::PopID();
		}

		ImGui::EndTable();
		ImGui::End();
	}
}