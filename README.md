# Engine

Engine is my game engine built to learn and implement the in-depth workings of game engines. This repository contains two primary modules:

- **Engine:** The core module responsible for rendering and fundamental engine functionalities.
- **Editor:** A GUI module for creating and controlling scenes, making it easier to design and manage your projects.

## Current Features

- **Model Loading:** Efficiently load and manage 3D models.
- **Shader Compilation:** Utilizes SPIRV, Shaderc, and glslang for compiling shaders and automatically retrieving uniforms.
- **Materials and Material Instancing:** Robust material system with instancing capabilities.
- **Undo/Redo Manager:** Easily revert or apply changes during development.
- **Windowing Support:** Multi-platform support including GLFW, X11, and Win32.
- **OpenGL Support:** Provides full OpenGL rendering support (Vulkan support will be added in the future).
- **Entity Component System (ECS):** Modular design for managing game entities.
- **Scene Hierarchy:** Organize and manage scene entities effectively.
- **Custom Event Handler:** Flexible system to manage and dispatch custom events.
- **Rendering Command Queue:** Efficient queuing of rendering commands for optimal performance.

## Vendors

Engine leverages several open-source libraries and tools to power its features:

- [Assimp](https://github.com/assimp/assimp)
- [Entt](https://github.com/skypjack/entt)
- [Glad](https://github.com/Dav1dde/glad)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- [Glslang](https://github.com/KhronosGroup/glslang)
- [ImGui](https://github.com/ocornut/imgui)
- [Shaderc](https://github.com/google/shaderc)
- [Spdlog](https://github.com/gabime/spdlog)
- [Spirv-Cross, Spirv-tools, Spirv-Headers](https://github.com/KhronosGroup/SPIRV-Cross)
- [stb](https://github.com/nothings/stb)

## Future Plans

- **Rendering Pass:** Implement multiple rendering passes to enhance visual effects.
- **Rendering Pipeline:** Develop a more robust and flexible rendering pipeline.
- **Improved Asset Loading:** Enhance the asset loading system for better performance and flexibility.
- **Scripting Support:** Introduce scripting capabilities for C++ and C# to extend engine functionality.
- **PBR (Physically Based Rendering):** Implement PBR for more realistic material and lighting effects.
- **Asset Management:** Build a comprehensive system for managing game assets.
- **Shadows:** Integrate dynamic and realistic shadow rendering.
- **Automatic Batch Rendering:** Optimize rendering performance through automatic batching.
- **Automatic Instancing:** Streamline rendering by automatically instancing similar objects.