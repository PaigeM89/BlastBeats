#pragma once
#include <Helpers.h>
#include <string>
#include <memory>
#include <ShlObj_core.h>
#include <ShObjIdl_core.h>
#include <WindowsCallbacks.h>

std::wstring FileOpenDialog()
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

std::string WCharToUtf8(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string utf8str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &utf8str[0], size_needed, NULL, NULL);
    return utf8str;
}

std::wstring GetUserDirPath()
{
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);
    std::wstring localAppDataPath{};
    if (SUCCEEDED(hr)) {
        localAppDataPath = path;
        CoTaskMemFree(path); // Free the memory allocated by the function
    }
    return localAppDataPath;
}

std::shared_ptr<Helpers::Callbacks> WindowsHelpers::CreateCallbacks()
{
    return std::make_shared<Helpers::Callbacks>(FileOpenDialog, WCharToUtf8, GetUserDirPath);
}