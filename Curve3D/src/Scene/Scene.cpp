#include "pch.h"
#include "Scene/Scene.h"

#include "Scene/SceneCamera.h"
#include "Scene/SceneParser.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Model.h"
#include "Scene/SceneSky.h"

#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/ResourceManager.h"

Scene::Scene(const std::string& sceneFilename)
	:m_sceneFilename(sceneFilename)
{
}
 
Scene::~Scene()
{
	m_sceneModels.clear();

	MeshManager::Get()->ClearAllResources();
	TextureManager::Get()->ClearAllResources();
	ShaderManager::Get()->ClearAllResources();
	CubemapManager::Get()->ClearAllResources();

	TheMaterialManager::Instance()->ClearAllMaterials();

	PRINT_GREEN("{0} has unloaded successfully", m_sceneFilename);
}

/// <summary>
/// Parses the XML scene file, creating all models and assets
/// </summary>
bool Scene::LoadScene()
{
	PRINT_WHITE("Attempting to load scene {0}", m_sceneFilename);

	// Create scene essentials
	m_sceneCamera = std::make_shared<SceneCamera>();
	m_sceneLightManager = std::make_shared<SceneLightManager>();

	// Parse the XML scene file to create scene models and resources
	SceneParser sceneParser;
	if (!sceneParser.ParseSceneFile(m_sceneFilename, m_sceneModels, m_sceneLightManager, &m_sceneSky))
		return false;

	// Setup object pointers with scene data
	m_sceneSky->SetCameraPointer(m_sceneCamera);
	TheMaterialManager::Instance()->SetAllMaterialScenePointers(m_sceneLightManager, m_sceneCamera);

	for (const auto& model : m_sceneModels)
	{
		model->SetModelPointers(m_sceneCamera);
	}

	PRINT_GREEN("{0} has loaded successfully", m_sceneFilename);
	return true;
}

/// <summary>
/// Update scene models and camera
/// </summary>
void Scene::UpdateScene()
{
	m_sceneCamera->Update();

	for (const auto& model : m_sceneModels)
	{
		model->UpdateModel();
	}
}

/// <summary>
/// Draw all scene models and the sky
/// </summary>
void Scene::RenderScene()
{
	for (const auto& m : m_sceneModels)
	{
		m->DrawModel();
	}

	if (m_sceneSky)
		m_sceneSky->DrawSky();
}

/// <summary>
/// Return the model at index within the SceneModels vector
/// </summary>
std::weak_ptr<Model> Scene::GetModelAtIndex(unsigned int index)
{
	if (index < m_sceneModels.size())
		return m_sceneModels[index];
	else
		return {};
}

/// <summary>
/// Delete a model at index within the SceneModels vector
/// </summary>
void Scene::DeleteModelAtIndex(unsigned int index)
{
	if (index < m_sceneModels.size())
		m_sceneModels.erase(m_sceneModels.begin() + index);
}

/// <summary>
/// Move the scene camera to specified position
/// </summary>
void Scene::SetSceneCameraPosition(Vector3D newPosition)
{
	m_sceneCamera->SetPosition(newPosition);
}
