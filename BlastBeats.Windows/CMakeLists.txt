add_executable(BlastBeats.Windows)

target_compile_features(BlastBeats.Windows PRIVATE cxx_std_20)

target_include_directories(BlastBeats.Windows
    PUBLIC
        ../imgui
        ../imgui/misc/cpp
        ../imgui/backends
)

target_sources(BlastBeats.Windows
    PRIVATE
        WindowsCallbacks.cpp
        ../imgui/imgui_draw.cpp
        ../imgui/imgui_tables.cpp
        ../imgui/imgui_widgets.cpp
        ../imgui/imgui.cpp
        ../imgui/misc/cpp/imgui_stdlib.cpp
        ../imgui/backends/imgui_impl_sdl3.cpp
        ../imgui/backends/imgui_impl_sdlgpu3.cpp

        ../imgui/imconfig.h
        ../imgui/imgui_internal.h
        ../imgui/imstb_rectpack.h
        ../imgui/imstb_textedit.h
        ../imgui/imstb_truetype.h
        ../imgui/imgui.h
        ../imgui/misc/cpp/imgui_stdlib.h
        ../imgui/backends/imgui_impl_sdl3.h
        ../imgui/backends/imgui_impl_sdlgpu3.h
        ../imgui/backends/imgui_impl_sdlgpu3_shaders.h

        MainGui.cpp
        main.cpp

    PUBLIC
        FILE_SET HEADERS
        FILES
            WindowsCallbacks.h
            MainGui.h
)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

target_link_libraries(BlastBeats.Windows
    PRIVATE
        BlastBeats.Gui.Components
        SDL3::SDL3
)


