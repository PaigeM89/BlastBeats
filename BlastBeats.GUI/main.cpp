#pragma once
#include <stdio.h>
#include "MainGui.h"
#include <AppState.h>
#include <Songs.h>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <GuiState.h>

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
	printf("BLASTBEATS: Hello from Blastbeats!\n");
	std::wcout.imbue(std::locale(""));

	auto musicDirManager = std::make_shared<MusicDirectories::MusicDirectoryManager>();

	auto musicDirGuiState = std::make_shared<GuiState::MusicDirectoryState>();
	std::shared_ptr<Observers::MusicDirectoryObserver> obs = musicDirManager;
	musicDirGuiState->Observers::MusicDirectorySubject::Attach(musicDirManager);

	auto start = std::chrono::steady_clock::now();
	
	musicDirGuiState->SetDirPath(L"E:\\music");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (musicDirManager->IsLoadingSongs())
	{
		size_t countSoFar = musicDirManager->GetSongCount();
		printf("BLASTBEATS: Loading songs...(%i loaded so far)\n", countSoFar);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		musicDirManager->CheckForCompletedThreads();
	}
	printf("BLASTBEATS: Song loading complete!");
	

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	//printCoolBand(sl);

	//auto returnCode = MainGui::RunGui(guiState);
	
	return 0;
}