//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"

#include <d3d11.h>

class TriangleProgram : public mcg::SampleProgram
{
public:
	TriangleProgram(const HINSTANCE hInstance, const int nCmdShow);

protected:
	HRESULT initializeResources();
	void cleanResouces();
	void render();

private:
	ID3D11Buffer*  mPVertexBuffer;
	ID3D11VertexShader* mPVertexShader;
	ID3D11PixelShader* mPPixelShader;
	ID3D11InputLayout* mPInputLayout;
};
