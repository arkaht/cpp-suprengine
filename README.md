# Suprengine
**Suprengine is a C++ 3D game engine using OpenGL and SDL2 for my personal learning experience.**

It uses **OpenGL 3.3** for rendering and **SDL2** for window and inputs handling. It has **ImGui** built-in for custom editor tools directly accesible in games.

## Dependencies
### Project
+ C++20 compiler
+ OpenGL 3.3.0
+ CMake 3.11

### Libraries
+ [SDL2](https://github.com/libsdl-org/SDL) with **TTF**, **Image** and **Mixer** (the latter is not used for now)
+ [GLEW](https://github.com/nigels-com/glew)
+ [assimp](https://github.com/assimp/assimp)
+ [rapidjson](https://github.com/Tencent/rapidjson)
+ [ImGui](https://github.com/ocornut/imgui/)
+ [Curve-X](https://github.com/arkaht/cpp-curve-x)
+ [filewatch](https://github.com/ThomasMonkman/filewatch)

## Features
+ Project IDE-independent thanks to **CMake**
+ Default assets such as 3D primitives (i.e. cube, cylinder, sphere, arrow), shaders and textures.
+ Basic 3D rendering using OpenGL with shaders and ambient lighting
+ Built-in **ImGui** for game tooling

## Project Structure
This project only holds the engine code since it is de-coupled from games code.

**Folder structure:**
+ **`assets/`** contains default assets, such as mesh primitives and shaders, packaged for any game to use.
+ **`libs/`** contains all libraries (e.g. SDL2, assimp, GLEW, ImGui...) necessary for the engine to compile.
+ **`src/`** contains source files of the engine.
