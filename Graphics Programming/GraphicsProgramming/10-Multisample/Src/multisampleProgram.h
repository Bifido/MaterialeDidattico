//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"

#include "types.h"
#include "camera.h"

class MultisampleProgram : public mcg::SampleProgram
{
public:
	MultisampleProgram(const HINSTANCE hInstance, const int nCmdShow);

	HRESULT initializeResources();
	void preRender();
	void render();
	void cleanResouces();

	void keyPressed(WPARAM key, LPARAM param);
	void keyReleased(WPARAM key, LPARAM param);

private:
	ID3D11Buffer* mPVertexBuffer;
	ID3D11Buffer* mPIndexBuffer;
	ID3D11Buffer* mPTranfBuffer;
	ID3D11VertexShader* mPVertexShader;
	ID3D11PixelShader* mPPixelShader;
	ID3D11InputLayout* mPInputLayout;
	ID3D11DepthStencilState* mPDepthStencilState;

	mcg::Transformations transf;
	mcg::SphericCamera camera;

	bool keyState[10]; //WASD
};
