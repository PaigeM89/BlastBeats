#include "MusicDirectories.h"
#include <memory>

std::shared_ptr<MusicDirectories::MusicDirectory> MusicDirectories::CreateMusicDirectory(std::wstring dirPath)
{
    return std::make_shared<MusicDirectories::MusicDirectory>(dirPath);
}
