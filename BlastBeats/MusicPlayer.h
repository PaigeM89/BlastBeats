#pragma once
#include "Songs.h"
#include <SFML/Audio.hpp>

namespace MusicPlayer {

	class Player {
	private:
		sf::Music music;
		std::shared_ptr<Songs::Song> m_CurrentSong;
		void StartCurrentSong();
	public:
		void SetAndPlaySong(std::shared_ptr<Songs::Song> song);
		void Play();
		void Stop();
		std::shared_ptr<Songs::Song> GetCurrentSong();
	};
}