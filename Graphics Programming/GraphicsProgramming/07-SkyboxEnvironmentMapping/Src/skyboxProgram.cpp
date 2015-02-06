//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "skyboxProgram.h"
#include "buffer.h"
#include "timer.h"
#include "shaderLoader.h"

#include "DDSTextureLoader.h"
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "DXUtils.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "DirectXTK.lib")

SkyboxProgram::SkyboxProgram(HINSTANCE hInstance, int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mTransfCBuffer(nullptr)
	, mLightsCBuffer(nullptr)
	, mDepthStateOn(nullptr)
	, mDepthStateOff(nullptr)
	, mTextDrawer(nullptr)
	, mArialFont(nullptr)
	, pauseLightsRotation(false)
	, mUseVenusModel(false)
{
	for(int i = 0; i < MAX_KEY; ++i)
		keys[i] = false;

	mModelWorld = XMMatrixIdentity();
	mSkyboxWorld = XMMatrixIdentity();
}


HRESULT SkyboxProgram::initializeResources()
{
	HRESULT result = S_OK; 

	// Creazione del costant buffer per le trasformazioni.
	mCamera.translate(-2.0f);
	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();
	mTransforms.world = XMMatrixIdentity();
	mTransforms.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), getWindowAspectRatio(), 0.1f, 100.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mTransforms, D3D11_USAGE_DEFAULT, &mTransfCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione di un constant buffer con le luci.
	// Numero di luci nella scena.
	mLights.usedLights = 2;

	// Prima luce
	mLights.position[0] = XMFLOAT4(4.0f, 0.0f, 0.0f, 1.0f);
	mLights.color[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mLights.ambient[0] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mLights.attenuation[0] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	// Seconda luce
	mLights.position[1] = XMFLOAT4(3.0f, -3.0f, 0.0f, 1.0f);
	mLights.color[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mLights.ambient[1] = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mLights.attenuation[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mLights, D3D11_USAGE_DEFAULT, &mLightsCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del depth stencil desc.
	D3D11_DEPTH_STENCIL_DESC dsDescON;
	dsDescON.DepthEnable = true;
	dsDescON.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescON.DepthFunc = D3D11_COMPARISON_LESS;
	dsDescON.StencilEnable = false;

	result = mPd3dDevice->CreateDepthStencilState(&dsDescON, &mDepthStateOn);
	if( FAILED( result ) )
        return result;

	D3D11_DEPTH_STENCIL_DESC dsDescOFF;
	dsDescOFF.DepthEnable = false;
	dsDescOFF.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDescOFF.DepthFunc = D3D11_COMPARISON_LESS;
	dsDescOFF.StencilEnable = false;

	result = mPd3dDevice->CreateDepthStencilState(&dsDescOFF, &mDepthStateOff);
	if( FAILED( result ) )
        return result;

	result =  mSkybox.initAndLoadCubeTexture(mPd3dDevice, L"./07-SkyboxEnvironmentMapping/skybox.dds");
	if( FAILED( result ) )
        return result;

	if(!mVenusReflectiveModel.initialize(mPd3dDevice, "./07-SkyboxEnvironmentMapping/venus.stl"))
		return E_FAIL;

	if(!mSphereReflectiveModel.initialize(mPd3dDevice, "./07-SkyboxEnvironmentMapping/sphere.stl"))
		return E_FAIL;

	mVenusReflectiveModel.setEnvironmentMap(mSkybox.getCubeTexture());
	mSphereReflectiveModel.setEnvironmentMap(mSkybox.getCubeTexture());

	mModelWorld = XMMatrixScaling(2.0f, 2.0f, 2.0f);

	//////////////////
	mLightSphere = std::unique_ptr<DirectX::GeometricPrimitive>(GeometricPrimitive::CreateSphere(mPd3dDeviceContext, 1.0f, 10));
	mTextDrawer = new mcg::TextDrawer(mPd3dDeviceContext);
	mArialFont = new mcg::TextFont(mPd3dDevice, L"./Data/arial16.spritefont");
	mTextDrawer->setColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 10.0f));
	//////////////////

	return result;
}

void SkyboxProgram::preRender()
{
	static mcg::Timer timer;
	float fSec = timer.elapsedSecF();
	timer.start();

	// Aggiorniamo la camera.
	// Input
	XMFLOAT4 cameraRotationAxe(0.0f, 0.0f, 0.0f, 0.0f);
	if(keys[ROTATE_CAMERA_LEFT])
		mCamera.yRotation(fSec);
	if(keys[ROTATE_CAMERA_RIGHT])
		mCamera.yRotation(-fSec);
	if(keys[ROTATE_CAMERA_UP])
		mCamera.xRotation(-fSec);
	if(keys[ROTATE_CAMERA_DOWN])
		mCamera.xRotation(fSec);
	if(keys[MOVE_CAMERA_FOREWARD])
		mCamera.translate(-fSec);
	if(keys[MOVE_CAMERA_BACKWARD])
		mCamera.translate(fSec);

	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();

	// Aggiorniamo le luci.
	if(!pauseLightsRotation)
	{
		XMMATRIX lightRotation = XMMatrixRotationY(fSec);
		XMVECTOR newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[0]), lightRotation);
		XMStoreFloat4(&mLights.position[0], newLightPosition);

		XMVECTOR lightRotationAxe = {1.0f, 1.0f, 0.0f, 0.0f};
		lightRotation = XMMatrixRotationAxis(lightRotationAxe, 1.2f * fSec);
		newLightPosition = XMVector4Transform(XMLoadFloat4(&mLights.position[1]), lightRotation);
		XMStoreFloat4(&mLights.position[1], newLightPosition);

		mPd3dDeviceContext->UpdateSubresource( mLightsCBuffer, 0, nullptr, &mLights, 0, 0 );
	}
}

void SkyboxProgram::render()
{
	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f );

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	mPd3dDeviceContext->OMSetDepthStencilState(mDepthStateOff, 0);

	mTransforms.world = mSkyboxWorld;
	mPd3dDeviceContext->UpdateSubresource( mTransfCBuffer, 0, nullptr, &mTransforms, 0, 0 );
	mSkybox.render(mPd3dDeviceContext, mTransfCBuffer);

	mPd3dDeviceContext->OMSetDepthStencilState(mDepthStateOn, 0);

	mTransforms.world = mModelWorld;
	mPd3dDeviceContext->UpdateSubresource( mTransfCBuffer, 0, nullptr, &mTransforms, 0, 0 );

	if(mUseVenusModel)
		mVenusReflectiveModel.render(mPd3dDeviceContext, mTransfCBuffer, mLightsCBuffer);
	else
		mSphereReflectiveModel.render(mPd3dDeviceContext, mTransfCBuffer, mLightsCBuffer);
	
	/////////////////////////////
	for(int i = 0; i < mLights.usedLights; ++i)
	{
		XMMATRIX lightWorld = XMMatrixTranslation(mLights.position[i].x, mLights.position[i].y, mLights.position[i].z);
		lightWorld = XMMatrixMultiply(XMMatrixScaling(0.3f, 0.3f, 0.3f), lightWorld);
		mLightSphere->Draw(lightWorld, mTransforms.view, mTransforms.projection, XMLoadFloat4(&mLights.color[i]));
	}

	// Sys REF
	float cPos = 3.0f;
	XMMATRIX sysWorld = XMMatrixMultiply(XMMatrixScaling(0.3f, 0.3f, 0.3f), XMMatrixTranslation(cPos, 0.0f, 0.0f));
	mLightSphere->Draw(sysWorld, mTransforms.view, mTransforms.projection, Colors::Red);

	sysWorld = XMMatrixMultiply(XMMatrixScaling(0.3f, 0.3f, 0.3f), XMMatrixTranslation(0.0f, cPos, 0.0f));
	mLightSphere->Draw(sysWorld, mTransforms.view, mTransforms.projection, Colors::Green);

	sysWorld = XMMatrixMultiply(XMMatrixScaling(0.3f, 0.3f, 0.3f), XMMatrixTranslation(0.0f, 0.0f, cPos));
	mLightSphere->Draw(sysWorld, mTransforms.view, mTransforms.projection, Colors::Blue);

	mTextDrawer->beginDraw();
	mTextDrawer->endDraw();
	////////////////////////////

	mPSwapChain->Present( 0, 0 );
}

void SkyboxProgram::keyPressed(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case VK_SPACE: mUseVenusModel = !mUseVenusModel; break;
		case 0x50: pauseLightsRotation = !pauseLightsRotation; break; // P
		case 0x41: keys[ROTATE_CAMERA_LEFT] = true; break; // A
		case 0x44: keys[ROTATE_CAMERA_RIGHT] = true; break; // D
		case 0x57: keys[ROTATE_CAMERA_UP] = true; break; // W
		case 0x53: keys[ROTATE_CAMERA_DOWN] = true; break; // S
		case 0x51: keys[MOVE_CAMERA_FOREWARD] = true; break; // Q
		case 0x45: keys[MOVE_CAMERA_BACKWARD] = true; break; // E
		default: SampleProgram::keyPressed(key, param);
	}
}

void SkyboxProgram::keyReleased(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case 0x41: keys[ROTATE_CAMERA_LEFT] = false; break; // A
		case 0x44: keys[ROTATE_CAMERA_RIGHT] = false; break; // D
		case 0x57: keys[ROTATE_CAMERA_UP] = false; break; // W
		case 0x53: keys[ROTATE_CAMERA_DOWN] = false; break; // S
		case 0x51: keys[MOVE_CAMERA_FOREWARD] = false; break; // Q
		case 0x45: keys[MOVE_CAMERA_BACKWARD] = false; break; // E
		default: SampleProgram::keyReleased(key, param);
	}
}

void SkyboxProgram::cleanResouces()
{
	if(mTransfCBuffer)
	{
		mTransfCBuffer->Release();
		mTransfCBuffer = nullptr;
	}

	if(mLightsCBuffer)
	{
		mLightsCBuffer->Release();
		mLightsCBuffer = nullptr;
	}

	if(mDepthStateOn)
	{
		mDepthStateOn->Release();
		mDepthStateOn = nullptr;
	}

	if(mDepthStateOff)
	{
		mDepthStateOff->Release();
		mDepthStateOff = nullptr;
	}
}
