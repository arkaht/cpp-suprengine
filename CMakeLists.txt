cmake_minimum_required(VERSION 3.11)

#  Setup Suprengine
project(SUPRENGINE)
set(CMAKE_CXX_STANDARD 20)
set(SUPRENGINE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SUPRENGINE_SOURCE "${SUPRENGINE_INCLUDE}/suprengine")
set(SUPRENGINE_ASSETS "${SUPRENGINE_SOURCE}/assets" CACHE INTERNAL "")

message("Suprengine Source Directory: '${SUPRENGINE_SOURCE_DIR}'")

#  List all .cpp files
file(GLOB_RECURSE SUPRENGINE_SOURCES CONFIGURE_DEPENDS "${SUPRENGINE_SOURCE}/*.cpp")
message("Suprengine Source Files: '${SUPRENGINE_SOURCES}'")

#  List all .dll files
file(GLOB_RECURSE SUPRENGINE_DLLS CONFIGURE_DEPENDS "${SUPRENGINE_SOURCE_DIR}/libs/*.dll")
list(FILTER SUPRENGINE_DLLS EXCLUDE REGEX "x86/")

#  Re-apply SUPRENGINE_DLLS variable to be accessible outside of this file scope
set(SUPRENGINE_DLLS "${SUPRENGINE_DLLS}" CACHE INTERNAL "")
message("Suprengine DLLs: '${SUPRENGINE_DLLS}'")

#  Declare library with sources and headers
add_library(SUPRENGINE)
set_target_properties(SUPRENGINE PROPERTIES OUTPUT_NAME "suprengine")
target_include_directories(SUPRENGINE PUBLIC "${SUPRENGINE_INCLUDE}")
target_sources(SUPRENGINE PRIVATE "${SUPRENGINE_SOURCES}")

#add_subdirectory("libs")
#target_link_libraries(SUPRENGINE PUBLIC sdl2)

#  Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(SUPRENGINE PUBLIC "${OPENGL_LIBRARIES}")

#  Link SDL2
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
target_link_libraries(SUPRENGINE PUBLIC 
	sdl2 sdl2_ttf sdl2_image 
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/SDL2-2.0.12/lib/x64/SDL2main.lib"  #  Required for the main function
)

#  Link GLEW
add_library(glew SHARED IMPORTED)
set_target_properties(glew PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/libs/glew-2.1.0/include"
)
target_link_libraries(SUPRENGINE PUBLIC glew)

#  Link ImGui
add_subdirectory("libs/imgui")
target_link_libraries(SUPRENGINE PUBLIC imgui)

#  Link assimp
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

# Copy suprengine DLLs to the specified project's binary directory
macro(suprengine_copy_dlls project_target)
	if(NOT SUPRENGINE_DLLS)
		message(FATAL_ERROR "Couldn't find Suprengine DLLs!")
	endif()
	if(NOT ${project_target}_BINARY_DIR)
		message(FATAL_ERROR "Please set variable '${project_target}_BINARY_DIR'!")
	endif()

	foreach(dll_path ${SUPRENGINE_DLLS})
		add_custom_command(TARGET ${project_target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll_path}" "${${project_target}_BINARY_DIR}"
		)
	endforeach()
endmacro()

# Copy suprengine assets as well as specified project assets to its binary directory
macro(suprengine_copy_assets project_target project_name)
	if(NOT ${project_target}_ASSETS)
		message(FATAL_ERROR "Please set variable '${project_target}_ASSETS'!")
	endif()
	if(NOT ${project_target}_BINARY_DIR)
		message(FATAL_ERROR "Please set variable '${project_target}_BINARY_DIR'!")
	endif()

	add_custom_command(TARGET ${project_target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${SUPRENGINE_ASSETS}" "${${project_target}_BINARY_DIR}/assets/suprengine"
	)
	add_custom_command(TARGET ${project_target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${${project_target}_ASSETS}" "${${project_target}_BINARY_DIR}/assets/${project_name}"
	)
endmacro()

#  Declare test executable
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory("src/demos/test")
	message("Included Suprengine test")
else()
	message("Skipped Suprengine test")
endif ()