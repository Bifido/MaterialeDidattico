//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"
#include "types.h"
#include "camera.h"
#include "textDrawer.h"
#include "skybox.h"
#include "reflectiveMeshModel.h"

#include "GeometricPrimitive.h"
#include <memory>

class SkyboxProgram : public mcg::SampleProgram
{
public:
	SkyboxProgram(HINSTANCE hInstance, int nCmdShow);

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

	ID3D11Buffer* mTransfCBuffer;
	ID3D11Buffer* mLightsCBuffer;
	ID3D11DepthStencilState* mDepthStateOn;
	ID3D11DepthStencilState* mDepthStateOff;

	Skybox mSkybox;
	ReflectiveMeshModel mVenusReflectiveModel;
	ReflectiveMeshModel mSphereReflectiveModel;

	DirectX::XMMATRIX mModelWorld;
	DirectX::XMMATRIX mSkyboxWorld;
	mcg::Transformations mTransforms;
	mcg::Lights mLights;

	mcg::SphericCamera mCamera;

	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;
	std::unique_ptr<DirectX::GeometricPrimitive> mLightSphere;

	bool keys[MAX_KEY];
	bool pauseLightsRotation;
	bool mUseVenusModel;
};
