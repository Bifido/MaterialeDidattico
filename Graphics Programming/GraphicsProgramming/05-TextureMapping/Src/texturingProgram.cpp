//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "texturingProgram.h"
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

TexturingProgram::TexturingProgram(HINSTANCE hInstance, int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mTransfCBuffer(nullptr)
	, mLightsCBuffer(nullptr)
	, mMaterialCBuffer(nullptr)
	, mDepthStencilState(nullptr)
	, mColorMapVS(nullptr)
	, mColorMapPS(nullptr)
	, mColorMapInputLayout(nullptr)
	, mColorTexResourceView(nullptr)
	, mColorTexSamplerState(nullptr)
	, mTextDrawer(nullptr)
	, mArialFont(nullptr)
	, pauseLightsRotation(false)
{
	for(int i = 0; i < MAX_KEY; ++i)
		keys[i] = false;
}


HRESULT TexturingProgram::initializeResources()
{
	HRESULT result = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mcg::PositionNormalTex2 vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, -1.0f, 0.0f ), XMFLOAT2( 0.0f, 1.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( -1.0f, 0.0f, 0.0f ), XMFLOAT2( 0.0f, 0.0f ) },

        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 1.0f, 0.0f, 0.0f ), XMFLOAT2( 1.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT3( 0.0f, 0.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },

        { XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT3( 0.0f, 0.0f, 1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
    };

	unsigned int indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

	// Creazione del vertex e index buffer
	result = mcg::createVertexBuffer(mPd3dDevice, vertices, 24, D3D11_USAGE_DEFAULT, &mVertexBuffer);
    if( FAILED( result ) )
        return result;

	result = mcg::createIndexBuffer(mPd3dDevice, indices, 36, D3D11_USAGE_DEFAULT, &mIndexBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del costant buffer per le trasformazioni.
	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();
	mTransforms.world = XMMatrixIdentity();
	mTransforms.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), getWindowAspectRatio(), 0.1f, 100.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mTransforms, D3D11_USAGE_DEFAULT, &mTransfCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del costant buffer per il materiale.
	mMaterial.ambiental = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	mMaterial.diffusive = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mMaterial.shininess = 128;

	result = mcg::createCostantBuffer(mPd3dDevice, &mMaterial, D3D11_USAGE_DEFAULT, &mMaterialCBuffer);
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

	result = mPd3dDevice->CreateDepthStencilState(&dsDescON, &mDepthStencilState);
	if( FAILED( result ) )
        return result;

	// Creazione degli shader per il color mapping
	ID3DBlob* vsColorMapBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./05-TextureMapping/ColorMapVS.cso", mPd3dDevice, &mColorMapVS, &vsColorMapBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsColorMapBlob, mPd3dDevice, layout, 3, &mColorMapInputLayout);
	if( FAILED( result ) )
        return result;

	vsColorMapBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./05-TextureMapping/ColorMapPS.cso", mPd3dDevice, &mColorMapPS);
	if( FAILED( result ) )
        return result;

	// Caricamento delle textures.
	result = DirectX::CreateDDSTextureFromFile(mPd3dDevice, L"./05-TextureMapping/colormap.dds", nullptr, &mColorTexResourceView);
	if( FAILED( result ) )
        return result;

	// Creiamo un sampler state.
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    result = mPd3dDevice->CreateSamplerState( &sampDesc, &mColorTexSamplerState );
    if( FAILED( result ) )
        return result;

	//////////////////
	mLightSphere = std::unique_ptr<DirectX::GeometricPrimitive>(GeometricPrimitive::CreateSphere(mPd3dDeviceContext, 1.0f, 10));
	mTextDrawer = new mcg::TextDrawer(mPd3dDeviceContext);
	mArialFont = new mcg::TextFont(mPd3dDevice, L"./Data/arial16.spritefont");
	mTextDrawer->setColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 10.0f));
	//////////////////

	return result;
}

void TexturingProgram::preRender()
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

	mPd3dDeviceContext->UpdateSubresource( mTransfCBuffer, 0, nullptr, &mTransforms, 0, 0 );

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

void TexturingProgram::render()
{
	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f );

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	mPd3dDeviceContext->OMSetDepthStencilState(mDepthStencilState, 0);
	
	// Settiamo il vertex buffer da renderizzare
	UINT stride = sizeof(mcg::PositionNormalTex2);
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mColorMapInputLayout);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mColorMapVS, nullptr, 0);
	mPd3dDeviceContext->PSSetShader(mColorMapPS, nullptr, 0);
	//Settiamo i constant buffer da utilizzare.
	mPd3dDeviceContext->VSSetConstantBuffers(0, 1, &mTransfCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(0, 1, &mMaterialCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(1, 1, &mLightsCBuffer);

	//Settiamo gli shader resource view e i sampler state
	mPd3dDeviceContext->PSSetShaderResources( 0, 1, &mColorTexResourceView );
	mPd3dDeviceContext->PSSetSamplers( 0, 1, &mColorTexSamplerState );

	mPd3dDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(36, 0, 0);

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

void TexturingProgram::keyPressed(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case VK_SPACE:  break;
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

void TexturingProgram::keyReleased(WPARAM key, LPARAM param)
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

void TexturingProgram::cleanResouces()
{
	if(mVertexBuffer)
		mVertexBuffer->Release();

	if(mIndexBuffer)
		mIndexBuffer->Release();
	
	if(mTransfCBuffer)
		mTransfCBuffer->Release();

	if(mLightsCBuffer)
		mLightsCBuffer->Release();

	if(mMaterialCBuffer)
		mMaterialCBuffer->Release();

	if(mDepthStencilState)
		mDepthStencilState->Release();

	if(mColorTexResourceView)
		mColorTexResourceView->Release();
}