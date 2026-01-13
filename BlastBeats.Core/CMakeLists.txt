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
            # Messages.h
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

find_package(ZLIB REQUIRED)

# this is an older version - might have to use a submodule after all
# find_package(taglib CONFIG REQUIRED)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/taglib")
# find_package(TagLib REQUIRED)

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

target_link_libraries(BlastBeats.Core 
    PUBLIC
        Uuids

    PRIVATE
        # ${CMAKE_SOURCE_DIR}/external/taglib/Release/tag.lib
        # taglib dependency, not sure this is needed
        ZLIB::ZLIB

        # i dunno man, just try shit, maybe something will work
        # tag
        # taglibdfdfdfdfd (this fails silently lol)
        TagLib

        # these are what the taglib docs say to link
        # taglib::tag
        # TagLib::tag_c
        # TagLib::TagLib

        SFML::Audio
)



target_compile_definitions(BlastBeats.Core PRIVATE TAGLIB_STATIC)

