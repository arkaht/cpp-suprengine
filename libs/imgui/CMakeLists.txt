add_library(IMGUI)
target_include_directories(IMGUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(IMGUI PRIVATE
    "imgui.cpp" "imgui_demo.cpp" "imgui_draw.cpp" "imgui_tables.cpp" "imgui_widgets.cpp"
    #  Referencing project backends
    "backends/imgui_impl_opengl3.cpp" "backends/imgui_impl_sdl2.cpp"
)
target_link_libraries(IMGUI PUBLIC SDL2)
