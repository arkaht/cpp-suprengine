cmake_minimum_required(VERSION 3.11)

project(TEST0)
set(CMAKE_CXX_STANDARD 20)
set(TEST0_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/")
set(TEST0_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/assets")

#  Find source files
file(GLOB_RECURSE TEST0_SOURCES CONFIGURE_DEPENDS "${TEST0_INCLUDE}/*.cpp")

#  Declare test project
add_executable(TEST0)
set_target_properties(TEST0 PROPERTIES OUTPUT_NAME "test")
target_include_directories(TEST0 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(TEST0 PRIVATE "${TEST0_SOURCES}")
target_link_libraries(TEST0 PRIVATE SUPRENGINE)

#  Copy DLLs and assets
suprengine_copy_dlls(TEST0)
suprengine_copy_assets(TEST0 "test")