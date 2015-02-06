//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "multisampleProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MultisampleProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"10 - Multisample");
	app.setWindowSize(640, 480);
	app.setMultisampleCount(8);

	return app.run();
}
