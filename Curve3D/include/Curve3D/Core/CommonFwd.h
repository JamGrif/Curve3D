#pragma once

// Forward declarations of typedefs and classes used in various files

// ResourceManager
typedef std::string ResourceFile;
typedef int ResourceID;

// IResource
constexpr ResourceID UNSET_RESOURCE_ID = 0;

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
constexpr auto NULL_TEXTURE = "null";

