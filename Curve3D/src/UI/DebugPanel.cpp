#include "pch.h"
#include "UI/DebugPanel.h"

#include "imgui.h"

#include "Scene/Scene.h"

DebugPanel::DebugPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer)
	:IPanel(panelName, imGuiWindowFlag, bVisible, pScenePointer)
{

}

DebugPanel::~DebugPanel()
{

}

void DebugPanel::Update()
{

}

void DebugPanel::Render()
{

}

void DebugPanel::SceneChange()
{

}
