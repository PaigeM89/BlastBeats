#pragma once
#include <Observer.h>
#include <memory>
#include <vector>
#include <string>
#include <Messages.h>

namespace GuiState {

	struct MusicDirectoryGuiState {
		//std::shared_ptr<MusicDirectories::MusicDirectory> MusicDirectory;
		std::wstring DirPath;
		/// <summary>
		/// The user has this directory currently selected in the UI.
		/// </summary>
		bool IsSelected;
		/// <summary>
		/// This directory is flagged to be removed from the directories that files are read from.
		/// This does NOT delete the directory from the system!
		/// </summary>
		bool FlaggedForRemoval;
	};

	class MusicDirectoryState : public Observers::Subject<Messages::MusicDirectoryChanged> {
	private:
		std::wstring m_DirPath{};
		bool m_IsSelected = false;
		bool m_FlaggedForRemoval = false;
	public:
		MusicDirectoryState() {};
		void SetDirPath(const std::wstring& dirPath);

		void Notify() override;
	};

	class GuiState {
	private:
		std::vector<std::shared_ptr<MusicDirectoryGuiState>> m_MusicDirectories;
	public:
		std::vector<std::shared_ptr<MusicDirectoryGuiState>> GetMusicDirectories();
	};

}