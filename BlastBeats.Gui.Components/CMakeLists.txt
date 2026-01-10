add_library(BlastBeats.Gui.Components)

target_sources(BlastBeats.Gui.Components
    PRIVATE
        MusicDirectoryGui.cpp
        MusicPlayerGui.cpp
        SongsTable.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            MusicDirectoryGui.h
            MusicPlayerGui.h
            SongsTable.h
            Helpers.h
)

target_link_libraries(BlastBeats.Gui.Components
    PRIVATE
        BlastBeats.Core
)

target_include_directories(BlastBeats.Gui.Components
    PUBLIC
        ../imgui
)