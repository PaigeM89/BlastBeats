#pragma once
#include <string>
#include "Uuid.h"

namespace Messages
{
	struct MusicDirectoryChanged
	{
		uuids::uuid Id;
		std::wstring DirPath;
		bool FlaggedForRemoval;
	};
}