#include "SongQueue.h"

std::optional<std::shared_ptr<Songs::Song>> SongQueue::Queue::GetFirstSong()
{
    if (m_QueuedSongs.size() > 0)
    {
        return m_QueuedSongs.at(0);
    }
    return {};
}

std::optional<std::shared_ptr<Songs::Song>> SongQueue::Queue::GetCurrentSong()
{
    if (m_QueuedSongs.size() > 0 && m_QueuedSongs.size() > m_CurrentSongIndex)
    {
        return m_QueuedSongs.at(m_CurrentSongIndex);
    }
    return {};
}

std::optional<std::shared_ptr<Songs::Song>> SongQueue::Queue::GetNextSong()
{
    if (m_QueuedSongs.size() > m_CurrentSongIndex + 1)
    {
        m_CurrentSongIndex++;
        return GetCurrentSong();
    }
    return {};
}

std::optional<std::shared_ptr<Songs::Song>> SongQueue::Queue::GetPreviousSong()
{
    if (m_QueuedSongs.size() == 0)
        return {};
    if (m_CurrentSongIndex == 0)
        return GetCurrentSong();

    m_CurrentSongIndex--;
    return GetCurrentSong();
}

std::vector<std::shared_ptr<Songs::Song>> SongQueue::Queue::GetQueue()
{
    return m_QueuedSongs;
}

void SongQueue::Queue::AddSongToFront(std::shared_ptr<Songs::Song> song)
{
    m_QueuedSongs.insert(m_QueuedSongs.begin(), song);
}

void SongQueue::Queue::AddSongsToFront(std::vector<std::shared_ptr<Songs::Song>>& songs)
{
    // go in reverse so that the last song in the collection is added first to the queue;
    // this puts the song at the top of the list passed in here to the front of the queue
    for (int i = songs.size() - 1; i >= 0; i--)
    {
        AddSongToFront(songs[i]);
    }
}

void SongQueue::Queue::AddSongToPlayNext(std::shared_ptr<Songs::Song> song)
{
    if (m_QueuedSongs.size() == 0)
    {
        AddSongToFront(song);
        return;
    }
    if (m_QueuedSongs.size() > m_CurrentSongIndex + 1)
    {
        m_QueuedSongs.insert(m_QueuedSongs.begin() + m_CurrentSongIndex, song);
        return;
    }
    m_QueuedSongs.push_back(song);
}

void SongQueue::Queue::AddSongsToPlayNext(std::vector<std::shared_ptr<Songs::Song>>& songs)
{
    for (int i = songs.size() - 1; i >= 0; i--)
    {
        AddSongToPlayNext(songs[i]);
    }
}

void SongQueue::Queue::AddSongToEnd(std::shared_ptr<Songs::Song> song)
{
    m_QueuedSongs.push_back(song);
}

void SongQueue::Queue::AddSongsToEnd(const std::vector<std::shared_ptr<Songs::Song>>& songs)
{
    for (const auto song : songs)
    {
        AddSongToEnd(song);
    }
}

void SongQueue::Queue::ClearQueue()
{
    m_QueuedSongs.clear();
    m_CurrentSongIndex = 0;
}


