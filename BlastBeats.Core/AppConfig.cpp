#pragma once
#include "AppConfig.h"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <iostream>

void FromJson(const nlohmann::json& j, AppConfig::ConfigFields& cf)
{
	j.at("musicDirs").get_to(cf.m_MusicDirs);
}

void FromJson2(const nlohmann::json& j, std::shared_ptr<AppConfig::ConfigFields> cf)
{
	j.at("musicDirs").get_to(cf->m_MusicDirs);
}

void ToJson(nlohmann::json& j, std::shared_ptr<AppConfig::ConfigFields> cf)
{
	j = nlohmann::json{ { "musicDirs", cf->m_MusicDirs } };
}

void CreateFolderInUserDirIfNotExists(std::filesystem::path& userDir)
{
	userDir /= "BlastBeats";
	if (!std::filesystem::exists(userDir))
	{
		std::filesystem::create_directories(userDir);
	}
}

void AppConfig::Config::LoadUserConfig()
{
	m_IsLoading = true;
	std::filesystem::path userDir(m_Callbacks->GetUserDirPath());

	if (userDir.empty())
		return;
	userDir /= "BlastBeats/BlastBeatsConfig.json";
	m_ConfigPath = userDir;

	if (!std::filesystem::exists(userDir))
	{
		std::wcout << "User config does not exist at location " << userDir << std::endl;
		m_ConfigFields = std::make_shared<AppConfig::ConfigFields>();
		return;
	}

	std::ifstream f(userDir);

	std::wcout << "Reading config file at location: " << userDir << std::endl;

	nlohmann::json data = nlohmann::json::parse(f);
	//AppConfig::ConfigFields cf{};
	auto cf = std::make_shared<AppConfig::ConfigFields>();
	FromJson2(data, cf);
	//m_ConfigFields = std::make_shared<AppConfig::ConfigFields>(cf);
	m_ConfigFields = cf;
	f.close();
}

void AppConfig::Config::WriteConfig()
{
	std::filesystem::path userDir(m_Callbacks->GetUserDirPath());
	CreateFolderInUserDirIfNotExists(userDir);
	if (userDir.empty())
		return;
	userDir /= "BlastBeatsConfig.json";
	m_ConfigPath = userDir;
	std::ofstream f(userDir);

	if (!f.is_open())
	{
		std::wcout << "Unable to open file for writing at location " << userDir << std::endl;
		return;
	}

	nlohmann::json j{};
	ToJson(j, this->m_ConfigFields);
	std::wcout << "Writing config file at location: " << userDir << std::endl;
	
	f << j << std::endl;
	
	f.close();
}

AppConfig::Config::Config(std::shared_ptr<Helpers::Callbacks> callbacks)
{
    m_Callbacks = callbacks;
	LoadUserConfig();
}

void AppConfig::Config::AddMusicDirectory(const std::wstring& musicDir)
{
	this->m_ConfigFields->m_MusicDirs.push_back(musicDir);
	this->WriteConfig();
}

std::vector<std::wstring> AppConfig::Config::GetMusicDirs()
{
	const auto& dirs = m_ConfigFields->m_MusicDirs;
	return dirs;
}
