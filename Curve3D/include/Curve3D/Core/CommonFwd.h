#pragma once

// Forward declares typedefs and classes/structs that are used in various files

// ResourceManager
typedef std::string ResourceFile;

// Internal OpenGL buffer index
typedef uint32_t OpenGLIndex;

typedef int ImGuiWindowFlags;

// Scene
class Scene;
class SceneCamera;
class SceneLightManager;
class SceneSky;

class Model;
typedef std::vector<std::shared_ptr<Model>> SceneModels;

// Material
constexpr int MATERIAL_TEXTURE_SLOTS = 5;
static const char* NULL_TEXTURE = "null";

