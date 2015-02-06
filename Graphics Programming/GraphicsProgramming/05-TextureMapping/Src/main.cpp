//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "texturingProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TexturingProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"05 - Texture Mapping");
	app.setWindowSize(640, 480);

	return app.run();
}
