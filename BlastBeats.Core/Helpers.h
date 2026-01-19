#pragma once
#include <functional>
#include <string>

namespace Helpers
{
    struct Callbacks
    {
        std::function<std::wstring(void)> FileOpenDialog;
        std::function<std::string(std::wstring)> WCharToUtf8;
        std::function<std::wstring()> GetUserDirPath;
    };
}