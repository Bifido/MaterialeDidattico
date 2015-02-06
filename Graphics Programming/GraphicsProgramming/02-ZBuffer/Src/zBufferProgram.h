//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"
#include <d3d11.h>

namespace mcg
{
	class TextDrawer;
	class TextFont;
}

class ZBufferProgram : public mcg::SampleProgram
{
public:
	ZBufferProgram(const HINSTANCE hInstance, const int nCmdShow);
	~ZBufferProgram();

	void keyPressed(WPARAM key, LPARAM param);

protected:
	HRESULT initializeResources();
	void cleanResouces();
	void render();

private:
	ID3D11Buffer* mPVertexBuffer;
	ID3D11Buffer* mPIndexBuffer00;
	ID3D11Buffer* mPIndexBuffer01;
	ID3D11VertexShader* mPVertexShader;
	ID3D11PixelShader* mPPixelShader;
	ID3D11InputLayout* mPInputLayout;
	ID3D11DepthStencilState* mPDepthStencilStateON;
	ID3D11DepthStencilState* mPDepthStencilStateOFF;
	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;

	bool mZBufferEnabled;
};
