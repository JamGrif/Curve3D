#include "pch.h"
#include "Core/Engine.h"

#include "Core/InputHandler.h"
#include "Core/UI.h"
#include "Scene/Scene.h"
#include "Rendering/OpenGLWindow.h"
#include "Rendering/OpenGLRenderer.h"

static constexpr int CONSOLE_STARTING_X = -900;
static constexpr int CONSOLE_STARTING_Y = 525;

static constexpr int CONSOLE_WIDTH = 900;
static constexpr int CONSOLE_HEIGHT = 500;

// Enable console if debug mode
#ifdef _DEBUG
	#include <windows.h>
	#define StartConsole()																								\
		AllocConsole();																									\
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);																\
		MoveWindow(GetConsoleWindow(), CONSOLE_STARTING_X, CONSOLE_STARTING_Y, CONSOLE_WIDTH, CONSOLE_HEIGHT, TRUE);
#else
	#define StartConsole()
#endif

namespace Curve3D
{
	static bool ENGINE_STARTED = false;
	static std::string sceneToLoad;

	void SetInitalParameters(const std::string& text)
	{
		sceneToLoad = text;

		ENGINE_STARTED = true;
	}

	void StartCurve3D()
	{
		if (!ENGINE_STARTED)
			return;

		ENGINE_STARTED = true;

		StartConsole();

		Program* em = new Program();

		if (em->Initialise(sceneToLoad))
			em->Loop();

		delete em;
	}

	Program::Program()
		:m_UI(nullptr), m_loadedScene(nullptr)
	{
	}

	Program::~Program()
	{
		if (m_loadedScene)
			delete m_loadedScene;

		if (m_UI)
			delete m_UI;

		TheOpenGLWindow::Get()->Clean();

		TheOpenGLRenderer::Get()->Clean();
	}

	/// <summary>
	/// Initializes OpenGL libraries, creates the window, enables rendering options and creates class objects
	/// Called once at program start
	/// </summary>
	bool Program::Initialise(const std::string& text)
	{
		// Initialize all systems used by Curve3D
		Log::Init();

		TheOpenGLRenderer::Get()->Init();

		Clock::Get()->Init();

		InputHandler::Get()->Init();

		m_UI = new UI(true, &m_loadedScene);

		if (!SetScene(text))
			return false;

		return true;
	}

	/// <summary>
	/// Entire loop of program
	/// Called every frame
	/// </summary>
	void Program::Loop()
	{
		while (!TheOpenGLWindow::Get()->ShouldClose())
		{
			Clock::Get()->Tick();

			HandleInput();
			UpdateApp();
			RenderApp();
		}
	}

	/// <summary>
	/// User input functions of the application
	/// </summary>
	void Program::HandleInput()
	{
		// Check if user wants to toggle UI visibility
		if (InputHandler::Get()->GetKeyPressedOnce(Keyboard::Q))
			m_UI->ToggleUI();
	}

	/// <summary>
	/// Update functions of the application
	/// </summary>
	void Program::UpdateApp()
	{
		if (m_loadedScene)
			m_loadedScene->UpdateScene();
	}

	/// <summary>
	/// Render functions of the application
	/// </summary>
	void Program::RenderApp()
	{
		TheOpenGLRenderer::Get()->StartOfFrame();

		if (m_loadedScene)
			m_loadedScene->RenderScene();

		if (m_UI)
			m_UI->RenderUI();

		TheOpenGLRenderer::Get()->EndOfFrame();
	}

	/// <summary>
	/// Changes the scene to specified scene name, unloads the currently loaded scene and refreshes the UI light buttons
	/// </summary>
	bool Program::SetScene(const std::string& sceneName)
	{
		// Prevent reloading current scene
		if (m_loadedScene && sceneName == m_loadedScene->GetSceneName())
			return false;

		if (m_loadedScene)
			delete m_loadedScene;

		m_loadedScene = new Scene(sceneName);

		if (m_loadedScene->LoadScene())
		{
			if (m_UI)
				m_UI->ChangePanelScene();

			// Scene successfully loaded
			return true;
		}

		// Scene failed to load
		return false;
	}
}
