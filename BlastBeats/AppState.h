#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Songs.h"
#include "MusicDirectories.h"

namespace AppState
{
	class ApplicationState {
	private:
		std::shared_ptr<MusicDirectories::MusicDirectoryManager> m_MusicDirectoryManager;
	public:
		ApplicationState();
		std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> GetMusicDirectories();
		std::vector<std::shared_ptr<Songs::Song>> GetSongs();
		void AddMusicDirectory(const std::wstring& dirPath);
		void RemoveDirectory(const std::wstring& directory);
	};
}