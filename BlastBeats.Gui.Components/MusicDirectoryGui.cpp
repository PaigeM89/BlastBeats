#pragma once
#include "MusicDirectoryGui.h"
#include "imgui.h"
#include <Windows.h>
#include <shobjidl.h> 
#include <sstream>
#include <locale>
#include <codecvt>

void MusicDirectoryGui::Render(std::shared_ptr<AppState::ApplicationState> appState, std::shared_ptr<Helpers::Callbacks> callbacks)
{
	ImGui::Begin("Music Directories");
	ImGui::BeginListBox("Source Directories");

    std::vector<int> selectedIndexes{};

    auto musicDirs = appState->GetMusicDirectories();

	for (int i = 0; i < musicDirs.size(); i++)
	{
        auto& x = musicDirs[i];
        const auto& dirPath = callbacks->WCharToUtf8(x->m_DirPath);
        if (ImGui::Selectable(dirPath.c_str(), x->m_IsSelected))
            x->m_IsSelected = !musicDirs[i]->m_IsSelected;

        if (x->m_IsSelected)
            selectedIndexes.push_back(i);
	}

	ImGui::EndListBox();
	if (ImGui::Button("AddMusicDir"))
	{
        auto dirToAdd = callbacks->FileOpenDialog();
        if (dirToAdd.length() > 0)
            appState->AddMusicDirectory(dirToAdd);
	}
    
    if (selectedIndexes.size() > 0)
    {
        if (ImGui::Button("Remove Selected"))
        {
            for (auto i : selectedIndexes)
            {
                musicDirs[i]->m_FlaggedForRemoval = true;
            }
        }
    }

	// todo: use a list box here, with some buttons
	// https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
	ImGui::End();
}
