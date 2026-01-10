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

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

target_compile_features(BlastBeats.Core PRIVATE cxx_std_20)

# target_link_libraries(BlastBeats.Core PRIVATE SFML::Audio)

find_package(ZLIB REQUIRED)
# this is an older version - might have to use a submodule after all
find_package(taglib CONFIG REQUIRED)

target_link_libraries(BlastBeats.Core 
    PUBLIC
        Uuids

    PRIVATE
        ZLIB::ZLIB
        TagLib::tag
        TagLib::tag_c
        TagLib::TagLib
        SFML::Audio
)

target_compile_definitions(BlastBeats.Core PRIVATE TAGLIB_STATIC)

