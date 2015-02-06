//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"
#include "camera.h"
#include "types.h"

#include "GeometricPrimitive.h"

#include <memory>

namespace mcg
{
	class TextDrawer;
	class TextFont;
}

struct ExplodeGS
{
	ExplodeGS() : explodeFactor(0.0f) {}

	float explodeFactor;
	float padding[3];
};

class GeometryProgram : public mcg::SampleProgram
{
public:
	GeometryProgram(const HINSTANCE hInstance, const int nCmdShow);

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
		INCREASE_EXPLODE_FACTOR,
		DECREASE_EXPLODE_FACTOR,
		MAX_KEY,
	};

	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;

	mcg::SphericCamera mCamera;

	mcg::Transformations mTransforms;
	mcg::Material mMaterial;
	mcg::Lights mLights;
	ExplodeGS mExplode;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mTranfCBuffer;
	ID3D11Buffer* mMaterialCBuffer;
	ID3D11Buffer* mLightsCBuffer;
	ID3D11Buffer* mExplodeCBuffer;
	ID3D11VertexShader* mPhongVS;
	ID3D11PixelShader* mPhongPS;
	ID3D11GeometryShader* mPhongGS;
	ID3D11InputLayout* mPhongInputLayout;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11RasterizerState* mRSNoCulling;

	bool pauseLightsRotation;
	bool keys[MAX_KEY];

	int mMeshIndices;
	std::unique_ptr<DirectX::GeometricPrimitive> mLightSphere;
};
