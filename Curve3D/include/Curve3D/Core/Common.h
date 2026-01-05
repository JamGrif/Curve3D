#pragma once

// ----- Forward declarations -----

// Scene
class Scene;
class SceneCamera;
class SceneLightManager;
class SceneSky;

class Model;

// ----- Typedefs -----

// ResourceManager
typedef std::string ResourceFile;
typedef int ResourceID;

// Internal OpenGL buffer index
typedef uint32_t OpenGLIndex;

typedef int ImGuiWindowFlags;

typedef std::vector<std::shared_ptr<Model>> SceneModels;


// ----- Magic Numbers -----

// Material
constexpr int MATERIAL_TEXTURE_SLOTS = 5;

// Resource
constexpr ResourceID UNSET_RESOURCE_ID = 0;

constexpr auto NULL_TEXTURE = "null";

