//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "geometryProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GeometryProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"11 - Geometry Shader");
	app.setWindowSize(640, 480);

	return app.run();
}
