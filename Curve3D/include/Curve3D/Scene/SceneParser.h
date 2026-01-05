#pragma once

class TiXmlElement;

struct ILightLoaderParams;
struct MaterialLoader;

/// <summary>
/// Parser reads in a XML file containing all of the scene information from res/scenes/
/// Parses the scene XML file and creates all objects, assets and light data the scene will use
/// </summary>
class SceneParser
{
public:
	SceneParser();
	~SceneParser() {}

	bool ParseSceneFile(const std::string& sceneFilepath, SceneModels& sceneModels, std::shared_ptr<SceneLightManager>& sceneLightManager, std::shared_ptr<SceneSky> sceneSky);

private:

	// Elements of .xml file
	void ReadMaterialsElement(const TiXmlElement* pMaterialsRootElement);
	void ReadModelsElement(const TiXmlElement* pModelsRootElement, SceneModels& sceneModels);
	void ReadLightsElement(const TiXmlElement* pLightsRootElement, std::shared_ptr<SceneLightManager>& sceneLightManager);

	void CreateResourceEssentials();

	void CreateMaterials();

	void ParseBaseLight(const TiXmlElement* lightNode, std::shared_ptr<ILightLoaderParams> pParams);

	typedef std::unordered_map<std::string, MaterialLoader> PendingMaterialPool;
	PendingMaterialPool m_materialMap;
};
