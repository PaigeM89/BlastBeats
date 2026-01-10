#include "MusicPlayer.h"
#include <SFML/Audio.hpp>
#include <fstream>

void MusicPlayer::Player::StartCurrentSong()
{
	std::filesystem::path path = std::filesystem::path(m_CurrentSong->GetFilepath());

	wprintf(L"Playing file: %s\n", m_CurrentSong->GetFilepath().c_str());
	
	if (!m_Music->openFromFile(path))
	{
		wprintf(L"Error playing file: %s\n", m_CurrentSong->GetFilepath().c_str());
		return;
	}
	m_Music->play();
}

void MusicPlayer::Player::SetAndPlaySong(std::shared_ptr<Songs::Song> song)
{
	m_CurrentSong = song;
	StartCurrentSong();
}

void MusicPlayer::Player::Play()
{
	if (m_CurrentSong == nullptr)
		return;
	StartCurrentSong();
}

void MusicPlayer::Player::Stop()
{
	m_Music->pause();
}

std::shared_ptr<Songs::Song> MusicPlayer::Player::GetCurrentSong()
{
	return m_CurrentSong;
}

MusicPlayer::Player::Player()
{
	m_Music = std::make_unique<sf::Music>();
}

MusicPlayer::Player::~Player()
{
	m_Music.release();
}
