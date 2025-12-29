#pragma once

#include "Rendering/Model.h"

class ModelManager
{
public:
	bool CreateModel(const std::string& modelID, const ModelLoader& pParams);


	static ModelManager* Instance()
	{
		static ModelManager* s_pInstance = new ModelManager;
		return s_pInstance;
	}
private:


	typedef std::unordered_map<std::string, std::shared_ptr<Model>> ModelPool;
	ModelPool	m_modelPool;

	ModelManager() {}
	~ModelManager() {}
	ModelManager(const ModelManager&) = delete;
};
typedef ModelManager TheModelManager;
