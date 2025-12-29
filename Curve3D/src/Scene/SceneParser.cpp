#include "pch.h"
#include "Scene/SceneParser.h"

#include "tinyXML/tinyxml.h"	// Read the level data

#include "Scene/SceneSky.h"
#include "Scene/SceneLightManager.h"
#include "Rendering/Resource/Manager/MaterialManager.h"
#include "Rendering/Resource/Manager/ResourceManager.h"
#include "Rendering/Model.h"

namespace
{
	constexpr uint8_t STRCMP_SUCCESS = 0;

	constexpr auto SCENE_FILEPATH_PREFIX = "res/scenes/";
	constexpr auto SCENE_FILEPATH_SUFFIX = ".xml";

	constexpr auto SKY_ATTRIBUTE = "skyid";

	// Scene Elements
	constexpr auto MATERIALS_ELEMENT = "materials";
	constexpr auto LIGHTS_ELEMENT = "lights";
	constexpr auto MODELS_ELEMENT = "models";

	// Scene Attributes


	// Material Attributes

	// Light Attributes


	// Model Attributes



}

// temp

std::string MISSING_TEXTURE_FILENAME1 = "missingtexture";
std::string MISSING_MESH_FILENAME = "missingmesh";

SceneParser::SceneParser()
{
}

/// <summary>
/// Parse the .xml scene file at sceneFilepath and fill the SceneModels vector, create all lights through the SceneLightManager and create the sky cubemap at sceneSky
/// </summary>
bool SceneParser::ParseSceneFile(const std::string& sceneFilepath, SceneModels& sceneModels, std::shared_ptr<SceneLightManager>& sceneLightManager, std::shared_ptr<SceneSky>* sceneSky)
{
	TiXmlDocument sceneDocument;

	// Load scene XML file and check status
	if (!sceneDocument.LoadFile(SCENE_FILEPATH_PREFIX + sceneFilepath + SCENE_FILEPATH_SUFFIX, TIXML_ENCODING_UTF8))
	{
		PRINT_RED("{0}", sceneDocument.ErrorDesc());
		return false;
	}

	// Store <scene> node
	TiXmlElement* pFileRoot = sceneDocument.RootElement();

	const TiXmlElement* pMaterialElement = nullptr, * pLightElement = nullptr, * pModelElement = nullptr;

	// Loop through all <scene> elements
	for (TiXmlElement* currentElement = pFileRoot->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
	{
		// <materials> element
		if (strcmp(currentElement->Value(), MATERIALS_ELEMENT) == STRCMP_SUCCESS)
		{
			pMaterialElement = currentElement;
			continue;
		}

		// <lights> element
		if (strcmp(currentElement->Value(), LIGHTS_ELEMENT) == STRCMP_SUCCESS)
		{
			pLightElement = currentElement;
			continue;
		}

		// <models> element
		if (strcmp(currentElement->Value(), MODELS_ELEMENT) == STRCMP_SUCCESS)
		{
			pModelElement = currentElement;
			continue;
		}
	}

	// Parse all the assets used in scene
	PerformanceTimer parseTimer("Asset Parsing");

	// Create sky using skyid as the cubemap filename
	*sceneSky = std::make_shared<SceneSky>(pFileRoot->Attribute(SKY_ATTRIBUTE));
	ResourceFile m_tempSkyCubemapID = pFileRoot->Attribute(SKY_ATTRIBUTE);
	CubemapLoader cubemapLoader;
	cubemapLoader.file = m_tempSkyCubemapID;
	CubemapManager::Get()->AddResource(&cubemapLoader);

	CreateResourceEssentials();

	// Parse each element of the scene file
	ReadMaterialsElement(pMaterialElement);
	ReadModelsElement(pModelElement, sceneModels);
	ReadLightsElement(pLightElement, sceneLightManager);


	CreateMaterials();

	



	parseTimer.Stop();

	// Create all assets used in scene
	PerformanceTimer creationTimer("Asset Creation");

	ShaderManager::Get()->CreateAllResources();
	TextureManager::Get()->CreateAllResources();
	MeshManager::Get()->CreateAllResources();
	CubemapManager::Get()->CreateAllResources();

	creationTimer.Stop();

	return true;
}

/// <summary>
/// Parse the <materials> node
/// </summary>
void SceneParser::ReadMaterialsElement(const TiXmlElement* pMaterialsElement)
{
	// Loop through all elements of <materials> node
	for (const TiXmlElement* materialNode = pMaterialsElement->FirstChildElement(); materialNode != NULL; materialNode = materialNode->NextSiblingElement())
	{
		// Ensure element is a <material> element
		if (strcmp(materialNode->Value(), "material") != STRCMP_SUCCESS)
			continue;

		// Fill out materials loading parameters
		MaterialLoader tempLoaderParams;

		materialNode->QueryStringAttribute("DiffuseFile",	&tempLoaderParams.textureFile[static_cast<int>(TextureType::DIFFUSE)]);
		materialNode->QueryStringAttribute("SpecularFile",	&tempLoaderParams.textureFile[static_cast<int>(TextureType::SPECULAR)]);
		materialNode->QueryStringAttribute("NormalFile",	&tempLoaderParams.textureFile[static_cast<int>(TextureType::NORMAL)]);
		materialNode->QueryStringAttribute("HeightFile",	&tempLoaderParams.textureFile[static_cast<int>(TextureType::HEIGHT)]);
		materialNode->QueryStringAttribute("EmissionFile",	&tempLoaderParams.textureFile[static_cast<int>(TextureType::EMISSION)]);

		m_materialMap.insert({ materialNode->Attribute("Name"), tempLoaderParams });
	}
}

/// <summary>
/// Parse the <lights> node
/// </summary>
void SceneParser::ReadLightsElement(const TiXmlElement* pLightsElement, std::shared_ptr<SceneLightManager>& sceneLightManager)
{
	// Loop through all elements of <lights> node
	for (const TiXmlElement* lightElement = pLightsElement->FirstChildElement(); lightElement != NULL; lightElement = lightElement->NextSiblingElement())
	{
		// Ensure element is a <light>
		if (strcmp(lightElement->Value(), "light") != STRCMP_SUCCESS)
			continue;
		
		// Create light depending on what type it is
		std::string_view type = lightElement->Attribute("type");
		if (type == "direction")
		{
			std::shared_ptr<DirectionalLoaderParams> tempParams = std::make_shared<DirectionalLoaderParams>();

			ParseBaseLight(lightElement, tempParams);

			lightElement->QueryFloatAttribute("dirX", &tempParams->direction.SetX());
			lightElement->QueryFloatAttribute("dirY", &tempParams->direction.SetY());
			lightElement->QueryFloatAttribute("dirZ", &tempParams->direction.SetZ());

			sceneLightManager->AddDirectionalLight(tempParams);
		}
		else if (type == "point")
		{
			std::shared_ptr<PointLoaderParams> tempParams = std::make_shared<PointLoaderParams>();

			ParseBaseLight(lightElement, tempParams);

			lightElement->QueryFloatAttribute("posX", &tempParams->position.SetX());
			lightElement->QueryFloatAttribute("posY", &tempParams->position.SetY());
			lightElement->QueryFloatAttribute("posZ", &tempParams->position.SetZ());

			sceneLightManager->AddPointLight(tempParams);
		}
		else if (type == "spot")
		{
			std::shared_ptr<SpotLoaderParams> tempParams = std::make_shared<SpotLoaderParams>();

			ParseBaseLight(lightElement, tempParams);

			lightElement->QueryFloatAttribute("posX", &tempParams->position.SetX());
			lightElement->QueryFloatAttribute("posY", &tempParams->position.SetY());
			lightElement->QueryFloatAttribute("posZ", &tempParams->position.SetZ());

			lightElement->QueryFloatAttribute("dirX", &tempParams->direction.SetX());
			lightElement->QueryFloatAttribute("dirY", &tempParams->direction.SetY());
			lightElement->QueryFloatAttribute("dirZ", &tempParams->direction.SetZ());

			sceneLightManager->AddSpotLight(tempParams);
		}
	}
}

/// <summary>
/// Parse the <models> node
/// </summary>
void SceneParser::ReadModelsElement(const TiXmlElement* pModelElement, SceneModels& sceneModels)
{
	std::unordered_multimap<std::string, std::string> tempMap;

	// Loop through all the elements of <models> node
	for (const TiXmlElement* modelElement = pModelElement->FirstChildElement(); modelElement != NULL; modelElement = modelElement->NextSiblingElement())
	{
		// Ensure element is a <model>
		if (strcmp(modelElement->Value(), "model") != STRCMP_SUCCESS)
			continue;

		// Fill out initial value of a model from XML scene data
		ModelLoader tempLoaderParams;

		modelElement->QueryStringAttribute("MaterialName",	&tempLoaderParams.materialName);
		modelElement->QueryStringAttribute("MeshFile",		&tempLoaderParams.meshFile);

		modelElement->QueryFloatAttribute("posX",		&tempLoaderParams.position.SetX());
		modelElement->QueryFloatAttribute("posY",		&tempLoaderParams.position.SetY());
		modelElement->QueryFloatAttribute("posZ",		&tempLoaderParams.position.SetZ());
		
		modelElement->QueryFloatAttribute("rotX",		&tempLoaderParams.rotation.SetX());
		modelElement->QueryFloatAttribute("rotY",		&tempLoaderParams.rotation.SetY());
		modelElement->QueryFloatAttribute("rotZ",		&tempLoaderParams.rotation.SetZ());

		modelElement->QueryFloatAttribute("scaleX",		&tempLoaderParams.scale.SetX());
		modelElement->QueryFloatAttribute("scaleY",		&tempLoaderParams.scale.SetY());
		modelElement->QueryFloatAttribute("scaleZ",		&tempLoaderParams.scale.SetZ());

		// Set models ID as number of times the mesh has currently been used in the scene
		tempMap.insert({ tempLoaderParams.meshFile , tempLoaderParams.meshFile });
		tempLoaderParams.modelID = tempLoaderParams.meshFile + " " + std::to_string(tempMap.count(tempLoaderParams.meshFile));

		sceneModels.emplace_back(std::make_shared<Model>(tempLoaderParams));

		// Use the meshID to create initial mesh
		MeshLoader meshLoader;
		meshLoader.file = tempLoaderParams.meshFile;
		MeshManager::Get()->AddResource(&meshLoader);
	}
}

/// <summary>
/// Create resources that must be made regardless of scene - such as missingtexture
/// </summary>
void SceneParser::CreateResourceEssentials()
{
	// Create missingtexture
	TextureLoader errorTextureLoader;
	errorTextureLoader.file = MISSING_TEXTURE_FILENAME1;
	errorTextureLoader.textureType = DIFFUSE;
	TextureManager::Get()->AddResource(&errorTextureLoader);

	// Create missingmesh
	// -
	MeshLoader errorMeshLoader;
	errorMeshLoader.file = MISSING_MESH_FILENAME;
	MeshManager::Get()->AddResource(&errorMeshLoader);

	// Create shader resource with shaderID
	ShaderLoader shaderLoader;
	shaderLoader.file = "lighting";
	ShaderManager::Get()->AddResource(&shaderLoader);
}

void SceneParser::CreateMaterials()
{
	// Create material using filled out loading parameters and assign the id to it
	for (const auto& [materialID, materialLoaderParams] : m_materialMap)
	{
		TheMaterialManager::Instance()->CreateMaterial(materialID, materialLoaderParams);
	}
}

/// <summary>
/// Fill out the base light loading parameters - all lights share these common values
/// </summary>
void SceneParser::ParseBaseLight(const TiXmlElement* lightNode, std::shared_ptr<ILightLoaderParams> pParams)
{
	lightNode->QueryFloatAttribute("ambR", &pParams->ambient.SetX());
	lightNode->QueryFloatAttribute("ambG", &pParams->ambient.SetY());
	lightNode->QueryFloatAttribute("ambB", &pParams->ambient.SetZ());

	lightNode->QueryFloatAttribute("difR", &pParams->diffuse.SetX());
	lightNode->QueryFloatAttribute("difG", &pParams->diffuse.SetY());
	lightNode->QueryFloatAttribute("difB", &pParams->diffuse.SetZ());

	lightNode->QueryFloatAttribute("speR", &pParams->specular.SetX());
	lightNode->QueryFloatAttribute("speG", &pParams->specular.SetY());
	lightNode->QueryFloatAttribute("speB", &pParams->specular.SetZ());
}
