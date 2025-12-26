#pragma once
#include <Songs.h>
#include "external/imgui/imgui.h"
#include <vector>

namespace GuiState {

	struct MusicDirectoryGuiState {
		std::shared_ptr<MusicDirectories::MusicDirectory> MusicDirectory;
		bool IsSelected;
		/// <summary>
		/// This directory is flagged to be removed from the directories that files are read from.
		/// This does NOT delete the directory from the system!
		/// </summary>
		bool FlaggedForRemoval;
	};

	class GuiState {
	private:
		std::vector<std::shared_ptr<MusicDirectoryGuiState>> m_MusicDirectories;
	public:
		std::vector<std::shared_ptr<MusicDirectoryGuiState>> GetMusicDirectories();
	};

}