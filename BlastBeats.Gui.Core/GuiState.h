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
		MusicDirectoryState() {
			Id = uuids::uuid_system_generator{}();
		};
		MusicDirectoryState(std::wstring& dirPath);
		std::wstring GetDirPath();
		void SetDirPath(const std::wstring& dirPath);
		void SetFlaggedForRemoval();
	};

	class RootGuiState {
	private:
		// todo: this needs to be aware of the blastbeats.core items, such as the music dir manager,
		// so that we can hook up the observers at the correct times.
		std::vector<std::shared_ptr<MusicDirectoryState>> m_MusicDirectories;
	public:
		std::vector<std::shared_ptr<MusicDirectoryState>> GetMusicDirectories();
		void AddMusicDirectory(std::wstring& dirPath);
	};

}