add_library(IMPLOT)
target_include_directories(IMPLOT PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(IMPLOT PRIVATE
    "implot.cpp" "implot_items.cpp" "implot_demo.cpp"
)
target_link_libraries(IMPLOT PUBLIC IMGUI)
