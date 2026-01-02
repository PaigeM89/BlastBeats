#pragma once
#include "external/imgui/imgui.h"
#include "SongsTable.h"
#include <Songs.h>
#include <AppState.h>
#include "WindowsHelpers.h"

static void RenderBasic(std::shared_ptr<AppState::ApplicationState> state)
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
}


void SongsTable::Render(std::shared_ptr<AppState::ApplicationState> state)
{
	static ImGuiTableFlags tableFlags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable
		| ImGuiTableFlags_Sortable | ImGuiTableFlags_HighlightHoveredColumn
		| ImGuiTableFlags_RowBg;

	static ImGuiSelectableFlags selectable_flags =
		ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;

	static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_None;

	if (ImGui::BeginTable("SongList", 4, tableFlags, ImVec2(0, 0)))
	{
		ImGui::TableSetupColumn("Title", column_flags);
		ImGui::TableSetupColumn("Album", column_flags);
		ImGui::TableSetupColumn("Artist", column_flags);
		ImGui::TableSetupColumn("Genre", column_flags);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		for (const auto& song : state->GetSongs())
		{
			ImGui::PushID(Helpers::WCharToUtf8(song->GetTitle()).c_str());
			const bool isSelected = song->p_IsSelected;
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);

			// this has to be a value in a column for the row selection to work.
			if (ImGui::Selectable(Helpers::WCharToUtf8(song->GetTitle()).c_str(), isSelected, selectable_flags, ImVec2(0, 0)))
			{
				wprintf(L"Toggling selection on song %s to %b", song->GetTitle(), !isSelected);
				song->p_IsSelected = !isSelected;
			}

			
			//ImGui::Text(Helpers::WCharToUtf8(song->GetTitle()).c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(Helpers::WCharToUtf8(song->GetAlbum()).c_str());
			ImGui::TableSetColumnIndex(2);
			ImGui::Text(Helpers::WCharToUtf8(song->GetArtist()).c_str());
			ImGui::TableSetColumnIndex(3);
			ImGui::Text(Helpers::WCharToUtf8(song->GetGenre()).c_str());

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	return;
}


