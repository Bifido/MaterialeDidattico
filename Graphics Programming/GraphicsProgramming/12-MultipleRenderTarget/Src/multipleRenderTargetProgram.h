//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"
#include "camera.h"
#include "types.h"
#include "textDrawer.h"

class MultipleRenderTarget : public mcg::SampleProgram
{
public:
	MultipleRenderTarget(const HINSTANCE hInstance, const int nCmdShow);

	HRESULT initializeResources();
	void preRender();
	void render();
	void cleanResouces();

	void keyPressed(WPARAM key, LPARAM param);
	void keyReleased(WPARAM key, LPARAM param);

private:
	HRESULT initializeSecondRenderTarget();
	HRESULT initializeFullScreenQuad();
	void renderingPass0();
	void renderingPass1();

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

	ID3D11Buffer* mPModelVertexBuffer;
	ID3D11Buffer* mPModelIndexBuffer;
	ID3D11Buffer* mTransfCBuffer;
	ID3D11Buffer* mLightsCBuffer;
	ID3D11Buffer* mMaterialCBuffer;
	ID3D11VertexShader* mPVertexShader;
	ID3D11PixelShader* mPPixelShader;
	ID3D11InputLayout* mPInputLayout;
	ID3D11DepthStencilState* mPDepthStencilState;

	ID3D11Buffer* mPQuadVertexBuffer;
	ID3D11VertexShader* mPQuadVertexShader;
	ID3D11PixelShader* mPQuadPixelShader;
	ID3D11InputLayout* mPQuadInputLayout;
	ID3D11SamplerState* mQuadTexSamplerState;

	ID3D11Buffer* mPixelInfoBuffer;
	ID3D11PixelShader* mPGaussianBlurPixelShader;

	ID3D11Texture2D* mSecondRenderTargetTextures[2];
	ID3D11RenderTargetView* mSecondRenderTargetView[2];
	ID3D11ShaderResourceView* mSecondShaderResourceView[2];

	mcg::Transformations mTransforms;
	mcg::Lights mLights;
	mcg::Material mMaterial;

	mcg::SphericCamera mCamera;

	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;

	unsigned int mNunberOfModelIndices;
	bool keys[MAX_KEY];
	bool pauseLightsRotation;
	bool showNormalTexture;
	bool blurImage;
};
