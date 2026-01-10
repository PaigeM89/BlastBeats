#pragma once
#include <functional>
#include <string>

namespace Helpers
{
    struct Callbacks
    {
        std::function<std::wstring(void)> FileOpenDialog;
        std::function<std::string(std::wstring)> WCharToUtf8;
    };

//#if _WIN32

    //static std::wstring FileOpenDialog()
    //{
    //    IFileOpenDialog* pFileOpen;
    //    PWSTR pszFilePath{};

    //    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
    //        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    //    if (SUCCEEDED(hr))
    //    {
    //        hr = pFileOpen->SetOptions(FOS_PICKFOLDERS);
    //        if (SUCCEEDED(hr))
    //        {
    //            // Show the Open dialog box.
    //            hr = pFileOpen->Show(NULL);

    //            // Get the file name from the dialog box.
    //            if (SUCCEEDED(hr))
    //            {
    //                IShellItem* pItem;
    //                hr = pFileOpen->GetResult(&pItem);
    //                if (SUCCEEDED(hr))
    //                {

    //                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
    //                }
    //                pItem->Release();
    //            }
    //            pFileOpen->Release();
    //        }
    //    }

    //    if (pszFilePath != nullptr)
    //    {
    //        return pszFilePath;
    //    }
    //    return std::wstring();
    //}

    //static std::string WCharToUtf8(const std::wstring& wstr)
    //{
    //    if (wstr.empty()) return std::string();
    //    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    //    std::string utf8str(size_needed, 0);
    //    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &utf8str[0], size_needed, NULL, NULL);
    //    return utf8str;
    //}
//#endif
}