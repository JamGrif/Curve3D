#pragma once
/*
	Engine.h
	Entry point to the engine
	Use Engine::StartEngine() to begin
*/ 

class UI;
class Scene;

#include <string>

namespace Curve3D
{
	void SetInitalParameters(const std::string& text);
	void StartCurve3D();

	/// <summary>
	/// Encapsulates the entire engine
	/// All parts of the engine are initialized, looped through and cleaned from this class
	/// </summary>
	class Program
	{
	private:
		Program();
		~Program();

		bool					Initialise(const std::string& text);
		void					Loop();

		void					HandleInput();
		void					UpdateApp();
		void					RenderApp();

		bool					SetScene(const std::string& sceneName);

		UI* m_UI;

		Scene* m_loadedScene;

		friend void StartCurve3D();
	};
}
