//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "zBufferProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ZBufferProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"02 - Z Buffer");
	app.setWindowSize(640, 480);

	return app.run();
}