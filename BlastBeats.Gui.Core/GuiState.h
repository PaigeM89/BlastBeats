#pragma once
#include <Observers.h>
#include <memory>
#include <vector>
#include <string>
#include <Messages.h>

namespace GuiState {

	class MusicDirectoryState : public Observers::MusicDirectorySubject
	{
	private:
		uuids::uuid Id;
		std::wstring m_DirPath{};
		bool m_FlaggedForRemoval = false;
	public:
		bool p_IsSelected = false;
		MusicDirectoryState() {};
		void SetDirPath(const std::wstring& dirPath);
		void SetFlaggedForRemoval();
	};

	class GuiState {
	private:
		std::vector<std::shared_ptr<MusicDirectoryState>> m_MusicDirectories;
	public:
		std::vector<std::shared_ptr<MusicDirectoryState>> GetMusicDirectories();
		void AddMusicDirectory(std::wstring dirPath);
	};

}