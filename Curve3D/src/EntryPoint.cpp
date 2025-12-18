#include "pch.h"

#include <windows.h>

#include "Core/Engine.h"

// Keep console open after program end (debug mode)
#ifdef _DEBUG
	#define KeepConsoleOpen() while (true) {}
#else
	#define KeepConsoleOpen() 
#endif

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
	// Convert PWSTR argument list into std::string
	std::wstring wideString = pCmdLine;
	std::string processArgument;
	processArgument.resize(wideString.length());

	wcstombs_s(NULL, &processArgument[0], processArgument.size() + 1, wideString.c_str(), wideString.size());
	
	if (processArgument.empty())
	{
		MessageBoxA(NULL, "Curve3D must be started from Curve3DLauncher!", "ERROR - :(", MB_OK | MB_ICONASTERISK);
		return 1;
	}

	Curve3D::SetInitalParameters(processArgument);
	Curve3D::StartCurve3D();
	
	//KeepConsoleOpen();

	return 0;
}
