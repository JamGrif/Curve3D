#include "pch.h"
#include "Rendering/Material.h"

#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Scene/SceneCamera.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/OpenGLRenderer.h"

#include "glm\gtc\matrix_transform.hpp"

std::string MISSING_TEXTURE_FILENAME2 = "missingtexture";

Material::Material(const MaterialLoader& pParams)
	:m_shaderID("lighting"),
	m_pAppProjectionMatrix(OpenGLRenderer::Get()->GetProjectionMatrix()),
	m_heightMapHeight(0.04f)
{
	// Step through each texture slot and attempt to load each required texture
	for (int textureSlot = 0; textureSlot != MATERIAL_TEXTURE_SLOTS; textureSlot++)
	{
		// Check if texture slot is used in material
		if (pParams.textureFile[textureSlot] == NULL_TEXTURE)
		{
			// Force missingtexture if empty diffuse slot
			if (textureSlot == DIFFUSE)
				m_materialTextures[textureSlot] = TextureManager::Get()->GetErrorResourceID();
			else
				m_materialTextures[textureSlot] = UNSET_RESOURCE_ID;

			continue;
		}

		// Setup texture to load
		TextureLoader textureLoader;
		textureLoader.file = pParams.textureFile[textureSlot];
		textureLoader.textureType = TextureType(textureSlot);

		// Attempt to create texture for specified texture slot
		ResourceID textureID = TextureManager::Get()->AddResource(&textureLoader);
		if (textureID == UNSET_RESOURCE_ID)
		{
			// Failed to create texture, so decided whether to disable texture slot or force missingtexture if diffuse slot
			if (textureLoader.textureType == DIFFUSE)
			{
				m_materialTextures[textureSlot] = TextureManager::Get()->GetErrorResourceID();
			}
			else
			{
				m_materialTextures[textureSlot] = UNSET_RESOURCE_ID;
			}
		}
		else
		{
			m_materialTextures[textureSlot] = textureID;
		}
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
		if (m_materialTextures[i] != UNSET_RESOURCE_ID)
			TextureManager::Get()->BindResourceAtID(m_materialTextures[i]);
	}

	// Bind the materials shader
	ShaderManager::Get()->BindResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID));

	std::shared_ptr<SceneCamera> tempSceneCamera = m_pSceneCamera.lock();
	std::shared_ptr<SceneLightManager> tempLightManager = m_pSceneLightManager.lock();

	// Set the values of the vertex shader
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("m_matrix", modelMat);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("v_matrix", tempSceneCamera->GetViewMatrix());
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("proj_matrix", m_pAppProjectionMatrix);

	// Set the values of the fragment shader
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.diffuse", DIFFUSE);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.specular", SPECULAR);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.normal", NORMAL);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.height", HEIGHT);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.emission", EMISSION);

	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.specularShininess", 48.0f);

	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.usingSpecular", m_materialTextures[SPECULAR]);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.usingNormal", m_materialTextures[NORMAL]);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.usingHeight", m_materialTextures[HEIGHT]);
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.usingEmission", m_materialTextures[EMISSION]);

	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("material.heightAmount", m_heightMapHeight);

	// Camera Position
	const Vector3D cameraPosition{ tempSceneCamera->GetPosition().x, tempSceneCamera->GetPosition().y, tempSceneCamera->GetPosition().z };
	ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("viewPos", cameraPosition);

	// Apply directional light values to shader
	if (tempLightManager->GetCurrentDirectionalLights() > 0) // Ensure a directional light exists
	{
		std::shared_ptr<DirectionalLight> tempDirectionalLight = tempLightManager->GetDirectionalLight(0).lock();

		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("dLight.ambient", tempDirectionalLight->m_ambient);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("dLight.diffuse", tempDirectionalLight->m_diffuse);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("dLight.specular", tempDirectionalLight->m_specular);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("dLight.direction", tempDirectionalLight->m_direction);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("dLight.lightActive", tempDirectionalLight->m_bLightActive);
	}

	// Apply point light(s) values to shader
	if (tempLightManager->GetCurrentPointLights() > 0) // Ensure a point light exists
	{
		for (unsigned int i = 0; i < tempLightManager->GetCurrentPointLights(); i++) // Loop through all scene point lights available
		{
			std::string s = std::to_string(i);
			std::shared_ptr<PointLight> tempPointLight = tempLightManager->GetPointLight(i).lock();

			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].ambient", tempPointLight->m_ambient);
			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].diffuse", tempPointLight->m_diffuse);
			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].specular", tempPointLight->m_specular);

			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].position", tempPointLight->m_position);
			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].constant", tempPointLight->m_constant);
			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].linear", tempPointLight->m_linear);
			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].quadratic", tempPointLight->m_quadratic);
			ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("pLight[" + s + "].lightActive", tempPointLight->m_bLightActive);
		}
	}

	// Apply spot light values to shader
	if (tempLightManager->GetCurrentSpotLights() > 0) // Ensure a spot light exists
	{
		std::shared_ptr<SpotLight> tempSpotLight = tempLightManager->GetSpotLight(0).lock();
		Vector3D cameraFront{ tempSceneCamera->GetFront().x, tempSceneCamera->GetFront().y, tempSceneCamera->GetFront().z };

		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.ambient", tempSpotLight->m_ambient);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.diffuse", tempSpotLight->m_diffuse);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.specular", tempSpotLight->m_specular);

		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.position", cameraPosition);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.direction", cameraFront);

		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.cutOff", glm::cos(glm::radians(tempSpotLight->m_cutOff)));
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.outerCutOff", glm::cos(glm::radians(tempSpotLight->m_outerCutOff)));

		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.constant", tempSpotLight->m_constant);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.linear", tempSpotLight->m_linear);
		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.quadratic", tempSpotLight->m_quadratic);

		ShaderManager::Get()->GetResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID))->SetUniform("sLight.lightActive", tempSpotLight->m_bLightActive);
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
		if (m_materialTextures[i])
			TextureManager::Get()->UnbindResourceAtID(m_materialTextures[i]);
	}
	
	ShaderManager::Get()->UnbindResourceAtID(ShaderManager::Get()->GetResourceIDFromFile(m_shaderID));
}

/// <summary>
/// Sets the scene specific pointers of the materials
/// </summary>
void Material::SetScenePointers(std::weak_ptr<SceneLightManager> pSceneLightManager, std::weak_ptr<SceneCamera> pSceneCamera)
{
	m_pSceneLightManager = pSceneLightManager;
	m_pSceneCamera = pSceneCamera;
}
