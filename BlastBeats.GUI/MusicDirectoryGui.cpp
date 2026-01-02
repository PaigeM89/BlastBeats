#pragma once
#include "MusicDirectoryGui.h"
#include "external/imgui/imgui.h"
#include <Windows.h>
#include <shobjidl.h> 
#include <sstream>
#include <locale>
#include <codecvt>
#include "AppState.h"

static std::wstring FileOpenDialog()
{
    IFileOpenDialog* pFileOpen;
    PWSTR pszFilePath{};

    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr))
    {
        hr = pFileOpen->SetOptions(FOS_PICKFOLDERS);
        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {

                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                }
                pItem->Release();
            }
            pFileOpen->Release();
        }
    }

    if (pszFilePath != nullptr)
    {
        return pszFilePath;
    }
    return std::wstring();
}

// windows-only conversion!!
static std::string WCharToUtf8(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string utf8str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &utf8str[0], size_needed, NULL, NULL);
    return utf8str;
}

void MusicDirectoryGui::Render(std::shared_ptr<AppState::ApplicationState> appState)
{
	ImGui::Begin("Music Directories");
	ImGui::BeginListBox("Source Directories");

    std::vector<int> selectedIndexes{};

    auto musicDirs = appState->GetMusicDirectories();

	for (int i = 0; i < musicDirs.size(); i++)
	{
        auto& x = musicDirs[i];
        const auto& dirPath = WCharToUtf8(x->m_DirPath); // todo - will this mess up formatting? is utf8 "enough"?
        if (ImGui::Selectable(dirPath.c_str(), x->m_IsSelected))
            x->m_IsSelected = !musicDirs[i]->m_IsSelected;

        if (x->m_IsSelected)
            selectedIndexes.push_back(i);
	}

	ImGui::EndListBox();
	if (ImGui::Button("AddMusicDir"))
	{
        auto dirToAdd = FileOpenDialog();
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
