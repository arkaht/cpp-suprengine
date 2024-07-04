cmake_minimum_required(VERSION 3.11)

#  Suprengine
project(SUPRENGINE)
set(CMAKE_CXX_STANDARD 20)

set(SUPRENGINE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/suprengine")
set(SUPRENGINE_ASSETS "${SUPRENGINE_SOURCE}/assets")

#  List all .dll files
file(GLOB_RECURSE SUPRENGINE_DLLS CONFIGURE_DEPENDS "${SUPRENGINE_SOURCE_DIR}/libs/*.dll")
list(FILTER SUPRENGINE_DLLS EXCLUDE REGEX "x86/")

#  List all .cpp files
file(GLOB_RECURSE SUPRENGINE_SOURCES CONFIGURE_DEPENDS "${SUPRENGINE_SOURCE}/*.cpp")
#list(FILTER SUPRENGINE_SOURCES EXCLUDE REGEX "src/demos/*")

#  Declare library with sources and headers
add_library(SUPRENGINE)
target_include_directories(SUPRENGINE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_sources(SUPRENGINE PRIVATE "${SUPRENGINE_SOURCES}")

#add_subdirectory("libs")
#target_link_libraries(SUPRENGINE PUBLIC sdl2)

#  Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(SUPRENGINE PUBLIC "${OPENGL_LIBRARIES}")

#  Link SDL2
#target_include_directories(SUPRENGINE PUBLIC 
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/include"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/include"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer-2.0.4/include"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-2.0.5/include"
#)
#target_link_libraries(SUPRENGINE PUBLIC
#    "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/lib/x64/SDL2.lib"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf.lib"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_mixer-2.0.4/lib/x64/SDL2_mixer.lib"
# 	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-2.0.5/lib/x64/SDL2_image.lib"
#)
add_library(sdl2 SHARED IMPORTED)
set_target_properties(sdl2 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/lib/x64/SDL2.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/lib/x64/SDL2.lib"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/include"
)
add_library(sdl2_ttf SHARED IMPORTED)
set_target_properties(sdl2_ttf PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf.lib"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/include"
)
add_library(sdl2_image SHARED IMPORTED)
set_target_properties(sdl2_image PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-2.0.5/lib/x64/SDL2_image.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-2.0.5/lib/x64/SDL2_image.lib"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2_image-2.0.5/include"
)
target_link_libraries(SUPRENGINE PUBLIC sdl2 sdl2_ttf sdl2_image "${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/lib/x64/SDL2main.lib")

#  Link GLEW
#target_include_directories(SUPRENGINE PUBLIC "libs/glew-2.1.0/include")
#target_link_libraries(SUPRENGINE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32.lib")
add_library(glew SHARED IMPORTED)
set_target_properties(glew PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dlls/glew32.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.1.0/include"
)
target_link_libraries(SUPRENGINE PUBLIC glew)

#  Link ImGui
#target_include_directories(SUPRENGINE PUBLIC "libs/imgui")
add_subdirectory("libs/imgui")
target_link_libraries(SUPRENGINE PUBLIC imgui)

#  Link assimp
#target_link_libraries(SUPRENGINE PUBLIC sdl2 sdl2_ttf sdl2_image)
#target_include_directories(SUPRENGINE PUBLIC "libs/assimp-5.3.1/include")
#target_link_libraries(SUPRENGINE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp-5.3.1/lib/assimp-vc143-mt.lib")
add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dlls/assimp-vc143-mt.dll"
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp-5.3.1/lib/assimp-vc143-mt.lib"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/assimp-5.3.1/include"
)
target_link_libraries(SUPRENGINE PUBLIC assimp)

#  Link filewatch
target_include_directories(SUPRENGINE PUBLIC "libs/filewatch/include")

#  Link Curve-X
target_include_directories(SUPRENGINE PUBLIC "libs/curve-x/include")
target_link_libraries(SUPRENGINE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/curve-x/lib/curve-x.lib")


#  Utility macros

# Copy suprengine assets as well as specified project assets to its binary directory
macro(suprengine_copy_assets project_target project_name)
	add_custom_command(TARGET ${project_target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${SUPRENGINE_ASSETS}" "${${project_target}_BINARY_DIR}/assets/suprengine"
	)
	add_custom_command(TARGET ${project_target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${${project_target}_BINARY_DIR}/assets/${project_name}"
	)
endmacro()

# Copy suprengine DLLs to the specified project's binary directory
macro(suprengine_copy_dlls project_target)
	foreach(dll_path ${SUPRENGINE_DLLS})
		add_custom_command(TARGET ${project_target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll_path}" "${${project_target}_BINARY_DIR}"
		)
	endforeach()
endmacro()

#  Declare test executable
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	#file(GLOB_RECURSE SUPRENGINE_TEST_SOURCES CONFIGURE_DEPENDS "src/demos/test/*.cpp")

	#add_executable(SUPRENGINE-test "${CMAKE_CURRENT_SOURCE_DIR}/src/demos/test/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/demos/test/scenes/game-scene.cpp")
	#target_link_libraries(SUPRENGINE-test PRIVATE SUPRENGINE)
    add_subdirectory("src/demos/test")

	message("Included Suprengine test")
else()
	message("Skipped Suprengine test")
endif ()