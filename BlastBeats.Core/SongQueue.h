#pragma once
#include <vector>
#include <memory>
#include "Songs.h"

namespace SongQueue {
	class Queue {
	private:
		std::vector<std::shared_ptr<Songs::Song>> m_QueuedSongs{};
		size_t m_CurrentSongIndex = 0;
	public:
		/// <summary>
		/// Gets the first song in the queue.
		/// </summary>
		/// <returns>None if there are no songs in the queue.</returns>
		std::optional<std::shared_ptr<Songs::Song>> GetFirstSong();

		/// <summary>
		/// Gets the current song in the queue.
		/// </summary>
		/// <returns>
		/// Returns None if there are no songs in the queue.
		/// </returns>
		std::optional<std::shared_ptr<Songs::Song>> GetCurrentSong();
		/// <summary>
		/// Gets the next song in the queue.
		/// </summary>
		/// <returns>
		/// A pointer to the next song, or None if the end of the queue has been reached.
		/// Returns None if there are no songs in the queue.
		/// </returns>
		std::optional<std::shared_ptr<Songs::Song>> GetNextSong();
		/// <summary>
		/// Gets the first song in the queue.
		/// </summary>
		/// <returns>
		/// A pointer to the previous song, or the first song if the pointer is already at the front of the queue.
		/// Returns None if there are no songs in the queue.
		/// </returns>
		std::optional<std::shared_ptr<Songs::Song>> GetPreviousSong();

		std::vector<std::shared_ptr<Songs::Song>> GetQueue();

		void AddSongToFront(std::shared_ptr<Songs::Song> song);
		void AddSongsToFront(std::vector<std::shared_ptr<Songs::Song>>& songs);
		void AddSongToPlayNext(std::shared_ptr<Songs::Song> song);
		void AddSongsToPlayNext(std::vector<std::shared_ptr<Songs::Song>>& songs);
		void AddSongToEnd(std::shared_ptr<Songs::Song> song);
		void AddSongsToEnd(const std::vector<std::shared_ptr<Songs::Song>>& songs);
		void ClearQueue(); // todo: does the queue control the player, and if so, does this clear the currently playing song?
	};
}