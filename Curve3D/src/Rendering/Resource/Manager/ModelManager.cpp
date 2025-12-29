#include "pch.h"
#include "Rendering/Resource/Manager/ModelManager.h"

bool ModelManager::CreateModel(const std::string& modelID, const ModelLoader& pParams)
{
	// Create model and insert into modelPool
	m_modelPool.insert({ modelID, std::make_shared<Model>(pParams) });

	return true;
}
