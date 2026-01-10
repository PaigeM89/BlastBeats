#pragma once
#include "Songs.h"
// #include <SFML/Audio.hpp>

namespace sf {
	class Music;
}

namespace MusicPlayer {

	class Player {
	private:
		std::unique_ptr<sf::Music> m_Music;
		std::shared_ptr<Songs::Song> m_CurrentSong;
		void StartCurrentSong();
	public:
		void SetAndPlaySong(std::shared_ptr<Songs::Song> song);
		void Play();
		void Stop();
		std::shared_ptr<Songs::Song> GetCurrentSong();

		Player();
		~Player();
	};
}