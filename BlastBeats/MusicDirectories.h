#pragma once
#include <string>
#include <memory>

namespace MusicDirectories {

	struct MusicDirectory {
		std::wstring DirPath;
		bool FlaggedForRemoval;

		MusicDirectory(std::wstring dirPath) {
			DirPath = dirPath;
			FlaggedForRemoval = false;
		}
	};

	std::shared_ptr<MusicDirectory> CreateMusicDirectory(std::wstring dirPath);
}