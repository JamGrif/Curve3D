#pragma once
#include "UI/IPanel.h"


class DebugPanel :
    public IPanel
{
public:
	DebugPanel(const std::string& panelName, ImGuiWindowFlags imGuiWindowFlag, bool bVisible, Scene** pScenePointer);
	~DebugPanel();

	virtual void	Update() override;
	virtual void	Render() override;
	virtual void	SceneChange() override;


private:



};

