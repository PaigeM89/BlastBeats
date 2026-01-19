#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <Helpers.h>
#include <optional>

namespace AppConfig {

	struct ConfigFields
	{
		std::vector<std::wstring> m_MusicDirs{};
	};

	class Config {
	private:
		std::shared_ptr<Helpers::Callbacks> m_Callbacks;
		std::filesystem::path m_ConfigPath{};
		bool m_IsLoading = false;

		std::shared_ptr<ConfigFields> m_ConfigFields;

		void LoadUserConfig();

		void WriteConfig();
	public:
		Config(std::shared_ptr<Helpers::Callbacks> callbacks);
		void AddMusicDirectory(const std::wstring& musicDir);
		std::vector<std::wstring> GetMusicDirs();
	};

}