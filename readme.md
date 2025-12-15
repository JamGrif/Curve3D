<img src="media/banner4.jpg">

# Curve3D  

The goal of Curve3D is to build a simple level editor within a lightweight C++ 3D OpenGL engine. This project is a continuation of a previous project [**here**](https://github.com/JamGrif/OpenGLProject) and uses a variety of additional libraries to achieve this goal. 

Curve3D contains a collection of 3D graphics features and works by parsing a custom .xml scene file which contains the necessary components of a 3D scene. Curve3D uses a launcher, Curve3DLauncher, which is built in C# with Winforms to set various launch parameters, such as the initial scene to load.  

This project is an experiment of mine with an overall aim to further push my OpenGL, modern C++, C# and mathematical skills while also improving my general software design ability.  

<img src="media/banner6.jpg" width=300>
    
# Features
- ADS Blinn-Phong Light Reflection Model  
- Diffuse, Specular, Emission, Height & Normal texture maps  
- Multithreading for asset deserialization  
- Material system that combines textures, mesh, shader and cubemap files to create a model  
- Scene .xml loading during runtime  
- Load external 3D models, textures & shaders  
- ImGui interface with a scene editor (Change scene, adjust lights status, view and delete entity values)  
- Winforms launcher to set initial program parameters  
- Console logging system for error handling  
- Coloured lighting from Directional/Point/Spot lights  
- Skybox Cubemap  
- Reflection Environment Map & Refraction Environment Map  
- Mouse + keyboard input with camera movement on all 3 axis  

**OLD** Project Showcase Video - https://www.youtube.com/watch?v=QOnscYxgrNQ

<img src="media/banner5.jpg">

<img src="media/banner3.jpg">

<img src="media/banner2.jpg">

<img src="media/banner1.jpg">

## **Curve3D uses the following dependencies:**

[assimp](https://github.com/assimp/assimp)

[GLEW](https://github.com/nigels-com/glew)

[GLFW](https://github.com/glfw/glfw)

[glm](https://github.com/g-truc/glm)

[imgui](https://github.com/ocornut/imgui)

[spdlog](https://github.com/gabime/spdlog)

[stb_image](https://github.com/nothings/stb)
