//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "bumpMappingProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BumpMappingProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"06 - Normal Bump Mapping");
	app.setWindowSize(640, 480);

	return app.run();
}
