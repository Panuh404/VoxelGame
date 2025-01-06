# Voxel Game

A Minecraft-inspired voxel game built with OpenGL and written in C++. Voxel Game is developed primarily as a learning project, exploring graphics, data structures, and real-time rendering concepts.

- Voxel-based world: Infinite(ish) terrain generation using voxel concepts.
- Rendering with OpenGL: Leverages modern OpenGL for 3D graphics.
- Basic collision and movement: Allows simple player movement around the environment.
- Procedural terrain generation: Experiment with noise functions (e.g., Perlin noise or Simplex noise) to generate varied landscapes

## Getting Started
These instructions will help you build and run Voxel Game on your local machine for development and testing purposes.

### 1. Cloning the repository
Start by cloning the repository with `git clone --recursive https://github.com/Panuh404/VoxelGame.git`

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

### 2. Generate the project files
Run the `GenerateProject.bat` file found in `scripts` folder. This will generate a Visual Studio solution file and copy the pch to the submodules.

If changes are made, or if you want to regenerate project files, rerun the GenerateProject script.

### 3. Open Solution and Build
Open the generated file (VoxelGame.sln) and build the solution

## Controls
- `WASD` for movement
- `Shift` to run
- `Ctrl` to descend
- `Space` to ascend or jump
- `Left Mouse Button` to break block
- `Middle Mouse Button` to pick a block
- `Right Mouse Button` to place block
- `Esc` to open menu

## Dependencies
- [GLFW](https://github.com/glfw/glfw) - for managing the window
- [glad](https://github.com/Dav1dde/glad) - for loading OpenGL functions
- [glm](https://github.com/g-truc/glm) - a math library OpenGL
- [imgui](https://github.com/ocornut/imgui) - for creating the UI
- [lodepng](https://github.com/lvandeve/lodepng) - for loading PNG textures
- [miniaudio](https://github.com/mackron/miniaudio) - for audio
- [spdlog](https://github.com/gabime/spdlog) - for fast logging
- [FastNoiseLite](https://github.com/Auburn/FastNoiseLite) - for terrain generation

## Documentation
- [Weighted Blended Order-Independent Transparency](https://jcgt.org/published/0002/02/09/)
- [Frustum](https://iquilezles.org/articles/frustumcorrect/)