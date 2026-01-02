#pragma once
#include "external/imgui/imgui.h"
#include "SongsTable.h"
#include <Songs.h>
#include <AppState.h>

// windows-only conversion!!
static std::string WCharToUtf8(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string utf8str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &utf8str[0], size_needed, NULL, NULL);
	return utf8str;
}

void SongsTable::Render(std::shared_ptr<AppState::ApplicationState> state)
{
	if (ImGui::BeginTable("songsTable", 3))
	{
		for (const auto& song : state->GetSongs())
		{
			ImGui::TableNextColumn();
			ImGui::Text(WCharToUtf8(song->GetTitle()).c_str());
			ImGui::TableNextColumn();
			ImGui::Text(WCharToUtf8(song->GetArtist()).c_str());
			ImGui::TableNextColumn();
			ImGui::Text(WCharToUtf8(song->GetGenre()).c_str());
		}
		ImGui::EndTable();
	}
	return;
}
