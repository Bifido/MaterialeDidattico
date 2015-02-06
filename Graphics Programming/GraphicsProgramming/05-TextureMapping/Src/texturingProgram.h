//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"
#include "types.h"
#include "camera.h"
#include "textDrawer.h"

#include "GeometricPrimitive.h"
#include <memory>

class TexturingProgram : public mcg::SampleProgram
{
public:
	TexturingProgram(HINSTANCE hInstance, int nCmdShow);

	HRESULT initializeResources();
	void cleanResouces();
	void preRender();
	void render();

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
		MAX_KEY,
	};

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mTransfCBuffer;
	ID3D11Buffer* mLightsCBuffer;
	ID3D11Buffer* mMaterialCBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11VertexShader* mColorMapVS;
	ID3D11PixelShader* mColorMapPS;
	ID3D11InputLayout* mColorMapInputLayout;
	ID3D11ShaderResourceView* mColorTexResourceView;
	ID3D11SamplerState* mColorTexSamplerState;

	mcg::Transformations mTransforms;
	mcg::Material mMaterial;
	mcg::Lights mLights;

	mcg::SphericCamera mCamera;

	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;
	std::unique_ptr<DirectX::GeometricPrimitive> mLightSphere;

	bool keys[MAX_KEY];
	bool pauseLightsRotation;
};
