cmake_minimum_required(VERSION 3.28)
project(BlastBeats LANGUAGES CXX
    VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(Uuids INTERFACE)

target_include_directories(Uuids INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/uuids>
)

add_subdirectory(BlastBeats.Core)
target_link_libraries(BlastBeats.Core PRIVATE SFML::Audio)

# TODO: there has to be a better way to do this than an absolute path
include(D:/git/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(ZLIB REQUIRED)

target_link_libraries(BlastBeats.Core PRIVATE ZLIB::ZLIB)

