add_library(sdl2 SHARED IMPORTED)
set_target_properties(sdl2 PROPERTIES
	IMPORTED_IMPLIB "lib/x64/SDL2.lib"
	INTERFACE_INCLUDE_DIRECTORIES "include"
)