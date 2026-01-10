add_library(BlastBeats.Core)

target_sources(BlastBeats.Core
    PRIVATE
        AppState.cpp
        MusicDirectories.cpp
        MusicPlayer.cpp
        Songs.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            AppState.h
            Messages.h
            MusicDirectories.h
            MusicPlayer.h
            Songs.h
)

target_link_libraries(BlastBeats.Core
    PUBLIC
        Uuids
)
