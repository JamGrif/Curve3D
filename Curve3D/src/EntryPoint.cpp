#include "pch.h"

#include <windows.h>

#include "Core/Engine.h"

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
	// Convert LPWSTR argument list into std::string
	std::wstring wideString = pCmdLine;
	std::string processArgument;
	processArgument.resize(wideString.length());

	wcstombs_s(NULL, &processArgument[0], processArgument.size() + 1, wideString.c_str(), wideString.size());
	
	if (processArgument.empty())
	{
		MessageBoxW(NULL, L"Curve3D must be started from Curve3DLauncher!", L"ERROR :(", MB_OK | MB_ICONASTERISK);
		return 1;
	}

	Curve3D::SetInitalParameters(processArgument);
	Curve3D::StartCurve3D();
	
	return 0;
}
