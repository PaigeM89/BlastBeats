add_library(BlastBeats.Core)

target_sources(BlastBeats.Core
    PRIVATE
        AppConfig.cpp
        AppState.cpp
        MusicDirectories.cpp
        MusicPlayer.cpp
        Songs.cpp
        SongQueue.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            Helpers.h
            AppConfig.h
            AppState.h
            MusicDirectories.h
            MusicPlayer.h
            Songs.h
            SongQueue.h
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

find_package(ZLIB REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/external/taglib/include
)

find_library(TagLibPath NAMES taglib tag HINTS ${CMAKE_SOURCE_DIR}/external/taglib/lib)

if(NOT TagLibPath)
    message(FATAL_ERROR "Failed to find TagLib library!")
else()
    message(STATUS "Found TagLib library: ${TagLibPath}!")
    add_library(TagLib STATIC IMPORTED)
    set_target_properties(TagLib
        PROPERTIES
            IMPORTED_LOCATION ${TagLibPath}
    )
endif()

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(BlastBeats.Core 
    PUBLIC
        Uuids

    PRIVATE
        # taglib dependency
        ZLIB::ZLIB
        TagLib
        SFML::Audio
        nlohmann_json::nlohmann_json
)

target_compile_definitions(BlastBeats.Core PRIVATE TAGLIB_STATIC)

