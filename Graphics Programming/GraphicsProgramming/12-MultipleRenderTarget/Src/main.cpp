//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include <Windows.h>

#include "multipleRenderTargetProgram.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MultipleRenderTarget app(hInstance, nCmdShow);
	app.setWindowTitle(L"12 - Multiple Render Target");
	app.setWindowSize(640, 480);

	return app.run();
}
