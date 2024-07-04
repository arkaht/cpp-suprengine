project(TEST0)

#  Find source files
file(GLOB_RECURSE SUPRENGINE_TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

#  Declare test project
add_executable(TEST0)
set_target_properties(TEST0 PROPERTIES OUTPUT_NAME "test")
target_include_directories(TEST0 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(TEST0 PRIVATE "${SUPRENGINE_TEST_SOURCES}")
target_link_libraries(TEST0 PRIVATE SUPRENGINE)

#  Copy DLLs and assets
suprengine_copy_dlls(TEST0)
suprengine_copy_assets(TEST0 "test")