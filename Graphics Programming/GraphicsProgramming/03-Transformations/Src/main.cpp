//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "transformationsProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TransformationProgram app(hInstance, nCmdShow);
	app.setWindowTitle(L"03 - Transformations");
	app.setWindowSize(640, 480);

	return app.run();
}
