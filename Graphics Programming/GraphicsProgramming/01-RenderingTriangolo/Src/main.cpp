//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "triangleProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TriangleProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"01 - Rendering di un triangolo");
	app.setWindowSize(640, 480);

	return app.run();
}
