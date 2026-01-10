add_executable(BlastBeats.Windows)

target_sources(BlastBeats.Windows
    PRIVATE
        WindowsCallbacks.cpp
        main.cpp
    
    PUBLIC
        FILE_SET HEADERS
        FILES
            WindowsCallbacks.h
)

target_link_libraries(BlastBeats.Windows
    PRIVATE
        BlastBeats.Gui.Components
)

target_compile_features(BlastBeats.Windows PRIVATE cxx_std_20)

