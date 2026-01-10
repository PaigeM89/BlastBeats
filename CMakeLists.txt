cmake_minimum_required(VERSION 3.28)
project(BlastBeats LANGUAGES CXX
    VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(Uuids INTERFACE)

target_include_directories(Uuids INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/uuids>
)

add_subdirectory(BlastBeats.Core)
add_subdirectory(BlastBeats.Gui.Components)
add_subdirectory(BlastBeats.Windows)