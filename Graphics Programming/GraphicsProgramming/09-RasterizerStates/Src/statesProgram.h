//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"

#include "types.h"
#include "camera.h"

#include <string>

namespace mcg
{
	class TextDrawer;
	class TextFont;
}

class StatesProgram : public mcg::SampleProgram
{
public:
	StatesProgram(const HINSTANCE hInstance, const int nCmdShow);

	HRESULT initializeResources();
	void preRender();
	void render();
	void cleanResouces();

	void keyPressed(WPARAM key, LPARAM param);
	void keyReleased(WPARAM key, LPARAM param);

private:
	enum KEY_INDICES
	{
		ROTATE_CAMERA_LEFT = 0,
		ROTATE_CAMERA_RIGHT,
		ROTATE_CAMERA_UP,
		ROTATE_CAMERA_DOWN,
		MOVE_CAMERA_FOREWARD,
		MOVE_CAMERA_BACKWARD,
		ROTATE_OBJECT_LEFT,
		ROTATE_OBJECT_RIGHT,
		ROTATE_OBJECT_UP,
		ROTATE_OBJECT_DOWN,
		MAX_KEY,
	};

	void setCurrentRSState(ID3D11RasterizerState* currentState, const std::string& stateName = "");
	void setCurrentBlendState(ID3D11BlendState* state, const std::string& stateName = "" );

	ID3D11Buffer* mPVertexBuffer;
	ID3D11Buffer* mPIndexBuffer;
	ID3D11Buffer* mPTranfBuffer;
	ID3D11VertexShader* mPVertexShader;
	ID3D11PixelShader* mPPixelShader;
	ID3D11InputLayout* mPInputLayout;
	ID3D11Buffer* mPPlaneVertexBuffer;
	ID3D11Buffer* mPPlaneIndexBuffer;

	ID3D11DepthStencilState* mPDepthStencilStateON;
	ID3D11DepthStencilState* mPDepthStencilStateWriteOFF;

	ID3D11RasterizerState* mCurrentRasterizerState;

	ID3D11RasterizerState* mBackFaceCullingState;
	ID3D11RasterizerState* mFrontFaceCullingState;
	ID3D11RasterizerState* mNoFaceCullingState;
	ID3D11RasterizerState* mWireframeState;

	ID3D11BlendState* mCurrentBlendState;

	ID3D11BlendState* mBlendingStateON;
	ID3D11BlendState* mBlendingStateOFF;

	mcg::Transformations transf;
	mcg::SphericCamera mCamera;

	DirectX::XMMATRIX mCubeWorldMatrix;
	DirectX::XMMATRIX mPlaneWorldMatrix;

	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;

	bool keys[MAX_KEY];
	std::string mCurrentRSName;
	std::string mCurrentBSName;
};
