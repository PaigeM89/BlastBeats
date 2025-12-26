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
		std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> m_MusicDirectories{};
		std::vector<std::shared_ptr<Songs::Song>> m_Songs{};
	public:
		std::vector<std::shared_ptr<MusicDirectories::MusicDirectory>> GetMusicDirectories();
		void AddMusicDirectory(std::wstring dirPath);
		void RemoveDirectory(std::wstring directory);
		void RemoveDirectory(size_t index);
	};
}