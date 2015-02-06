//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "statesProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StatesProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"09 - Rasterizer States");
	app.setWindowSize(640, 480);

	return app.run();
}