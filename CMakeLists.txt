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

# find_library(TagLib NAMES taglib tag HINTS external/taglib/taglib/Release)

# if(TagLib)
#     message(STATUS "Found TagLib library!")
# else()
#     message(FATAL_ERROR "Failed to find TagLib library!")
# endif()

# add_library(TagLib STATIC IMPORTED)
# set_property(TARGET TagLib PROPERTY IMPORTED_LOCATION /external/taglib)

add_subdirectory(taglib)
add_subdirectory(BlastBeats.Core)
add_subdirectory(BlastBeats.Gui.Components)
add_subdirectory(BlastBeats.Windows)