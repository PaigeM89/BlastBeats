#pragma once
#include <stdio.h>
#include "MainGui.h"
#include <AppState.h>
#include <Songs.h>
#include <iostream>
#include <chrono>
#include <filesystem>

static void printCoolBand(const std::shared_ptr<Songs::SongList> songList)
{
	for (auto& song : songList->GetSongs())
	{
		if (!song->IsEmpty() && song->GetArtist() == L"1349")
		{
			std::wcout << song->GetTitle() << std::endl;
		}
	}
}

int main(int, char**) {
	printf("Hello from Blastbeats!\n");
	std::wcout.imbue(std::locale(""));
	auto start = std::chrono::steady_clock::now();
	auto p = std::filesystem::path("E:\\music");
	auto musicDir = MusicDirectories::CreateMusicDirectory(p);
	//auto songs = Songs::LoadAllSongsInDirectory(p);
	auto sl = Songs::LoadAllSongsInDirectory(musicDir);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Loaded " << sl->GetSongs().size() << " songs in " << elapsed_seconds.count() << " seconds." << std::endl;
	
	printCoolBand(sl);

	auto guiState = std::make_shared<GuiState::GuiState>();
	auto returnCode = MainGui::RunGui(guiState);
	
	return 0;
}