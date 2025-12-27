#pragma once
#include <string>
#include <memory>
#include "Uuid.h"
#include "Observers.h"
#include "Messages.h"

namespace MusicDirectories {

	struct MusicDirectory {
		uuids::uuid Id;
		std::wstring DirPath;
		bool FlaggedForRemoval;

		MusicDirectory(std::wstring dirPath) {
			Id = uuids::uuid();
			DirPath = dirPath;
			FlaggedForRemoval = false;
		}
	};

	class MusicDirectoryManager : Observers::Observer<Messages::MusicDirectoryChanged>
	{
	private:
		std::vector<std::shared_ptr<MusicDirectory>> m_MusicDirectories{};
	public:
		void AddDirectory(std::wstring& dirPath);
		void RemoveDirectory(std::wstring& directory);
		void RemoveDirectory(size_t index);
		void Update(std::shared_ptr<Messages::MusicDirectoryChanged> msg) override;
	};

	std::shared_ptr<MusicDirectory> CreateMusicDirectory(std::wstring dirPath);
}