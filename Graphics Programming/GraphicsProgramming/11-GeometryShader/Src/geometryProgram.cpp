//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "geometryProgram.h"
#include "timer.h"
#include "shaderLoader.h"
#include "buffer.h"
#include "textDrawer.h"
#include "stlMeshLoader.h"
#include "utils.h"

#include <sstream>


#pragma comment(lib, "DXUtils.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;


GeometryProgram::GeometryProgram(const HINSTANCE hInstance, const int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mPhongVS(nullptr)
	, mPhongPS(nullptr)
	, mPhongGS(nullptr)
	, mPhongInputLayout(nullptr)
	, mDepthStencilState(nullptr)
	, mMaterialCBuffer(nullptr)
	, mTranfCBuffer(nullptr)
	, mLightsCBuffer(nullptr)
	, mExplodeCBuffer(nullptr)
	, mRSNoCulling(nullptr)
	, pauseLightsRotation(false)
	, mMeshIndices(0)
{
	for(int i = 0; i < MAX_KEY; ++i)
		keys[i] = false;
}

HRESULT GeometryProgram::initializeResources()
{
	HRESULT result = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mcg::STLMeshLoader loader;
	if(!loader.loadFormFile("./11-GeometryShader/sphere.stl"))
		return E_FAIL;

	loader.swapHandness();
	loader.smoothNormals(15.0f);
	

	// Creazione del vertex e index buffer per il cubo
	if(!loader.getVertexBuffer(mPd3dDevice, &mVertexBuffer))
		return false;

	if(!loader.getIndexBuffer(mPd3dDevice, &mIndexBuffer))
		return false;

	mMeshIndices = loader.getNumberOfIndices();

	// Creazione del costant buffer per le trasformazioni.
	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();
	mTransforms.world = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	mTransforms.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), getWindowAspectRatio(), 0.1f, 100.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mTransforms, D3D11_USAGE_DEFAULT, &mTranfCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del costant buffer per il materiale.
	mMaterial.ambiental = XMFLOAT4(0.1f, 0.3f, 0.3f, 1.0f);
	mMaterial.diffusive = XMFLOAT4(0.2f, 0.5f, 0.5f, 1.0f);
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
	mLights.position[0] = XMFLOAT4(3.0f, 0.0f, 0.0f, 1.0f);
	mLights.color[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mLights.ambient[0] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mLights.attenuation[0] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	// Seconda luce
	mLights.position[1] = XMFLOAT4(3.0f, -3.0f, 0.0f, 1.0f);
	mLights.color[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mLights.ambient[1] = XMFLOAT4(0.3f, 0.1f, 0.1f, 1.0f);
	mLights.attenuation[1] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

	result = mcg::createCostantBuffer(mPd3dDevice, &mLights, D3D11_USAGE_DEFAULT, &mLightsCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del constant buffer per il geometry shader;
	result = mcg::createCostantBuffer(mPd3dDevice, &mExplode, D3D11_USAGE_DEFAULT, &mExplodeCBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione degli shader per Phong
	ID3DBlob* vsPhongBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./11-GeometryShader/PhongShadingVS.cso", mPd3dDevice, &mPhongVS, &vsPhongBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsPhongBlob, mPd3dDevice, layout, 2, &mPhongInputLayout);
	if( FAILED( result ) )
        return result;

	vsPhongBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./11-GeometryShader/PhongShadingPS.cso", mPd3dDevice, &mPhongPS);
	if( FAILED( result ) )
        return result;

	// Creazione del Geometry shader
	result = mcg::ShaderLoader::geometryPreBuiltLoad(L"./11-GeometryShader/PhongShadingGS.cso", mPd3dDevice, &mPhongGS);
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

	// Creazione di un rasterizer state per disabilitare il face culling.
	D3D11_RASTERIZER_DESC rsDesc;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = true;
	rsDesc.DepthBias = false;
	rsDesc.DepthBiasClamp = 0;
	rsDesc.SlopeScaledDepthBias = 0;
	rsDesc.DepthClipEnable = true;
	rsDesc.ScissorEnable = false;
	rsDesc.MultisampleEnable = false;

	result = mPd3dDevice->CreateRasterizerState(&rsDesc, &mRSNoCulling);
	if(FAILED(result))
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

void GeometryProgram::preRender()
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
	if(keys[INCREASE_EXPLODE_FACTOR])
		mExplode.explodeFactor += fSec / 2.0f;
	if(keys[DECREASE_EXPLODE_FACTOR])
		mExplode.explodeFactor -= fSec / 2.0f;

	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();

	mPd3dDeviceContext->UpdateSubresource( mTranfCBuffer, 0, nullptr, &mTransforms, 0, 0 );
	mPd3dDeviceContext->UpdateSubresource( mExplodeCBuffer, 0, nullptr, &mExplode, 0, 0 );

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

void GeometryProgram::render()
{
	//XMFLOAT4 clearColor(0.39f, 0.58f, 0.93f, 1.0f );
	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f );

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mPd3dDeviceContext->OMSetDepthStencilState(mDepthStencilState, 0);

	mPd3dDeviceContext->RSSetState(mRSNoCulling);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPhongInputLayout);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPhongVS, nullptr, 0);
	mPd3dDeviceContext->PSSetShader(mPhongPS, nullptr, 0);
	mPd3dDeviceContext->GSSetShader(mPhongGS, nullptr, 0);
	//Settiamo i constant buffer da utilizzare.
	mPd3dDeviceContext->VSSetConstantBuffers(0, 1, &mTranfCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(0, 1, &mMaterialCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(1, 1, &mLightsCBuffer);
	mPd3dDeviceContext->GSSetConstantBuffers(0, 1, &mTranfCBuffer);
	mPd3dDeviceContext->GSSetConstantBuffers(1, 1, &mExplodeCBuffer);

	// Disegno della sfera.
	// Settiamo il vertex buffer da renderizzare.
	UINT stride = sizeof(mcg::PositionNormal);
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mPd3dDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(mMeshIndices, 0, 0);


	/////////////////////////////
	mPd3dDeviceContext->GSSetShader(nullptr, nullptr, 0);

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
	std::stringstream textStream;
	textStream << "Explode Factor: " << mExplode.explodeFactor;
	mTextDrawer->drawText(*mArialFont, mcg::toWString(textStream.str()));
	mTextDrawer->endDraw();
	////////////////////////////


	mPSwapChain->Present( 0, 0 );
}

void GeometryProgram::keyPressed(WPARAM key, LPARAM param)
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
		case 0x5A: keys[DECREASE_EXPLODE_FACTOR] = true; break; // Z
		case 0x58: keys[INCREASE_EXPLODE_FACTOR] = true; break; // X
		default: SampleProgram::keyPressed(key, param);
	}
}

void GeometryProgram::keyReleased(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case 0x41: keys[ROTATE_CAMERA_LEFT] = false; break; // A
		case 0x44: keys[ROTATE_CAMERA_RIGHT] = false; break; // D
		case 0x57: keys[ROTATE_CAMERA_UP] = false; break; // W
		case 0x53: keys[ROTATE_CAMERA_DOWN] = false; break; // S
		case 0x51: keys[MOVE_CAMERA_FOREWARD] = false; break; // Q
		case 0x45: keys[MOVE_CAMERA_BACKWARD] = false; break; // E
		case 0x5A: keys[DECREASE_EXPLODE_FACTOR] = false; break; // Z
		case 0x58: keys[INCREASE_EXPLODE_FACTOR] = false; break; // X
		default: SampleProgram::keyReleased(key, param);
	}
}

void GeometryProgram::cleanResouces()
{
	if(mVertexBuffer)
		mVertexBuffer->Release();

	if(mIndexBuffer)
		mIndexBuffer->Release();

	if(mPhongVS)
		mPhongVS->Release();

	if(mPhongPS)
		mPhongPS->Release();

	if(mPhongGS)
		mPhongGS->Release();

	if(mPhongInputLayout)
		mPhongInputLayout->Release();

	if(mDepthStencilState)
		mDepthStencilState->Release();

	if(mMaterialCBuffer)
		mMaterialCBuffer->Release();

	if(mTranfCBuffer)
		mTranfCBuffer->Release();

	if(mLightsCBuffer)
		mLightsCBuffer->Release();

	if(mExplodeCBuffer)
		mExplodeCBuffer->Release();

	if(mRSNoCulling)
		mRSNoCulling->Release();
}
