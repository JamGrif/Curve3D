#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/OpenGLRenderer.h"

#include "glm\gtc\matrix_transform.hpp"


Material::Material(const MaterialLoader& pParams)
	:m_shaderID("lightingShader"),
	m_textureMapUsing{ false, false, false, false, false },
	m_pAppProjectionMatrix(OpenGLRenderer::Get()->GetProjectionMatrix()),
	m_heightMapHeight(0.01f)
{
	// Go through MaterialLoaderParams and set each texture type that is used
	for (int textureSlot = 0; textureSlot != MATERIAL_TEXTURE_SLOTS; textureSlot++)
	{
		// Check if texture slot is used in material
		if (pParams.textureFile[textureSlot] == NULL_TEXTURE)
		{
			m_textureFile[textureSlot] = NULL_TEXTURE;
			continue;
		}

		//	BELOW IS WHERE YOU WANT TO RETURN TRUE OR FALSE WITH ADDRESOURCE TO CHECK IF ITS SUCCESSFULLY MADE.
		// IF FALSE THEN NOT CREATE TEXTURE
		// CAN PROBS MERGE M_TEXTUREFILE AND TEXTUREMAPUSING
		// Create Texture resource with textureMapID

		TextureLoader textureLoader;
		textureLoader.file = pParams.textureFile[textureSlot];
		textureLoader.textureType = TextureType(textureSlot);

		TextureManager::Get()->AddResource(&textureLoader);
		//TextureManager::Get()->GetResourceAtID(pParams.textureFile[textureSlot])->setTextureType(static_cast<TextureType>(textureSlot));

		// Set if material is using each texture slot
		m_textureFile[textureSlot] = pParams.textureFile[textureSlot];
		m_textureMapUsing[textureSlot] = true;

	}

	// Create shader resource with shaderID
	ShaderManager::Get()->AddResource(m_shaderID, "res/shaders/lightingPassTwo-vertex.glsl", "res/shaders/lightingPassTwo-fragment.glsl");

	// DEBUG
	for (int i = 0; i != MATERIAL_TEXTURE_SLOTS; i++)
	{
		PRINT_INFO("Texture: {0}", m_textureFile[i]);
	}
}


Material::~Material()
{
}

/// <summary>
/// Bind material's textures and shader to the OpenGL context and sets all uniform values for the shader
/// </summary>
void Material::BindMaterial(const glm::mat4& modelMat)
{
	// Bind each used texture
	for (int i = 0; i != MATERIAL_TEXTURE_SLOTS; i++)
	{
		if (m_textureFile[i] != NULL_TEXTURE)
			TextureManager::Get()->BindResourceAtID(m_textureFile[i]);
	}

	// Bind the materials shader
	ShaderManager::Get()->BindResourceAtID(m_shaderID);

	std::shared_ptr<SceneCamera> tempSceneCamera = m_pSceneCamera.lock();
	std::shared_ptr<SceneLightManager> tempLightManager = m_pSceneLightManager.lock();

	// Set the values of the vertex shader
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("m_matrix", modelMat);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("v_matrix", tempSceneCamera->GetViewMatrix());
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("proj_matrix", m_pAppProjectionMatrix);

	// Set the values of the fragment shader
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.diffuse", static_cast<int>(TextureType::DIFFUSE));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.specular", static_cast<int>(TextureType::SPECULAR));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.emission", static_cast<int>(TextureType::EMISSION));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.normal", static_cast<int>(TextureType::NORMAL));
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.height", static_cast<int>(TextureType::HEIGHT));

	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.specularShininess", 48.0f);

	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.usingEmission", m_textureMapUsing[static_cast<int>(TextureType::EMISSION)]);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.usingNormal", m_textureMapUsing[static_cast<int>(TextureType::NORMAL)]);
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.usingHeight", m_textureMapUsing[static_cast<int>(TextureType::HEIGHT)]);

	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("material.heightAmount", m_heightMapHeight);

	// Camera Position
	const Vector3D cameraPosition{ tempSceneCamera->GetPosition().x, tempSceneCamera->GetPosition().y, tempSceneCamera->GetPosition().z };
	ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("viewPos", cameraPosition);

	// Apply directional light values to shader
	if (tempLightManager->GetCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = tempLightManager->GetDirectionalLight(0).lock();

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.ambient", tempDirectionalLight->m_ambient);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.diffuse", tempDirectionalLight->m_diffuse);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.specular", tempDirectionalLight->m_specular);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.direction", tempDirectionalLight->m_direction);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("dLight.lightActive", tempDirectionalLight->m_bLightActive);
	}

	// Apply point light(s) values to shader
	if (tempLightManager->GetCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < tempLightManager->GetCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = tempLightManager->GetPointLight(i).lock();

			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].ambient", tempPointLight->m_ambient);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].diffuse", tempPointLight->m_diffuse);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].specular", tempPointLight->m_specular);

			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].position", tempPointLight->m_position);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].constant", tempPointLight->m_constant);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].linear", tempPointLight->m_linear);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].quadratic", tempPointLight->m_quadratic);
			ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("pLight[" + s + "].lightActive", tempPointLight->m_bLightActive);
		}
	}

	// Apply spot light values to shader
	if (tempLightManager->GetCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = tempLightManager->GetSpotLight(0).lock();
		Vector3D cameraFront{ tempSceneCamera->GetFront().x, tempSceneCamera->GetFront().y, tempSceneCamera->GetFront().z };

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.ambient", tempSpotLight->m_ambient);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.diffuse", tempSpotLight->m_diffuse);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.specular", tempSpotLight->m_specular);

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.position", cameraPosition);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.direction", cameraFront);

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.constant", tempSpotLight->m_constant);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.linear", tempSpotLight->m_linear);
		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.quadratic", tempSpotLight->m_quadratic);

		ShaderManager::Get()->GetResourceAtID(m_shaderID)->SetUniform("sLight.lightActive", tempSpotLight->m_bLightActive);
	}
}

/// <summary>
/// Unbinds the material's textures and shader from the OpenGL context
/// </summary>
void Material::UnbindMaterial()
{
	// Unbind each available texture
	for (int i = 0; i != MATERIAL_TEXTURE_SLOTS; i++)
	{
		if (m_textureMapUsing[i])
			TextureManager::Get()->UnbindResourceAtID(m_textureFile[i]);
	}
	
	ShaderManager::Get()->UnbindResourceAtID(m_shaderID);
}

/// <summary>
/// Sets the scene specific pointers of the materials
/// </summary>
void Material::SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera)
{
	m_pSceneLightManager = pSceneLightManager;
	m_pSceneCamera = pSceneCamera;
}
