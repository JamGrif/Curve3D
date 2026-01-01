#pragma once

#include "glm/mat4x4.hpp"

/// <summary>
/// Material loading parameters which are passed to a Material object on construction to set initial values
/// </summary>
struct MaterialLoader
{
	std::string textureFile[MATERIAL_TEXTURE_SLOTS];
};

/// <summary>
/// Encapsulates multiple Texture objects and a single Shader object
/// Used in the Model class with a Mesh to render
/// </summary>
class Material
{
public:
	Material(const MaterialLoader& pParams);
	~Material();

private:

	void			BindMaterial(const glm::mat4& modelMat);
	void			UnbindMaterial();

	void			SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera);

private:

	// Shader the material uses
	std::string		m_shaderID;

	// Filename of the texture in texture slot
	typedef std::array<ResourceID, MATERIAL_TEXTURE_SLOTS> MaterialTextures;
	MaterialTextures m_materialTextures;

	// Rendering data
	float			m_heightMapHeight;

	// Cached scene objects
	std::weak_ptr<SceneLightManager> m_pSceneLightManager;
	std::weak_ptr<SceneCamera> m_pSceneCamera;
	const glm::mat4& m_pAppProjectionMatrix;

	// Ensures only the MaterialManager can call functions of a material object
	friend class MaterialManager;
};
