//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "multipleRenderTargetProgram.h"
#include "shaderLoader.h"
#include "timer.h"
#include "buffer.h"
#include "stlMeshLoader.h"

#pragma comment(lib, "DXUtils.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;

struct FullScreenQuadVertex
{
	XMFLOAT3 position;
	XMFLOAT2 texCoord;
};

MultipleRenderTarget::MultipleRenderTarget(const HINSTANCE hInstance, const int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mPModelVertexBuffer(nullptr)
	, mPModelIndexBuffer(nullptr)
	, mPVertexShader(nullptr)
	, mPPixelShader(nullptr)
	, mPInputLayout(nullptr)
	, mPDepthStencilState(nullptr)
	, mTransfCBuffer(nullptr)
	, mLightsCBuffer(nullptr)
	, mMaterialCBuffer(nullptr)
	, mNunberOfModelIndices(0)
	, pauseLightsRotation(false)
	, showNormalTexture(false)
	, blurImage(false)
	, mPQuadVertexBuffer(nullptr)
	, mPQuadVertexShader(nullptr)
	, mPQuadPixelShader(nullptr)
	, mPQuadInputLayout(nullptr)
	, mQuadTexSamplerState(nullptr)
	, mPixelInfoBuffer(nullptr)
	, mPGaussianBlurPixelShader(nullptr)
{
	for(int i = 0; i < MAX_KEY; ++i)
		keys[i] = false;

	for(unsigned int i = 0; i < 2; ++i)
	{
		mSecondRenderTargetTextures[i] = nullptr;
		mSecondRenderTargetView[i] = nullptr;
		mSecondShaderResourceView[i] = nullptr;
	}
}

HRESULT MultipleRenderTarget::initializeResources()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	HRESULT result = S_OK;

	mcg::STLMeshLoader model;
	if(!model.loadFormFile("./12-MultipleRenderTarget/venus.stl"))
		return E_FAIL;

	// Le mesh stl sono right-hand.
	model.swapHandness();
	// Smoothing delle normali (stl usa normali per faccia.
	// In questo modo ogni vertice avra' una normale continua con tutte le facce vicine).
	model.smoothNormals(25.0f);

	if(! model.getVertexBuffer(mPd3dDevice, &mPModelVertexBuffer))
		return E_FAIL;

	if(! model.getIndexBuffer(mPd3dDevice, &mPModelIndexBuffer))
		return E_FAIL;

	mNunberOfModelIndices = model.getNumberOfIndices();

	ID3DBlob* vsBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./12-MultipleRenderTarget/PhongShadingVS.cso", mPd3dDevice, &mPVertexShader, &vsBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsBlob, mPd3dDevice, layout, 2, &mPInputLayout);
	if( FAILED( result ) )
        return result;

	vsBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./12-MultipleRenderTarget/PhongShadingPS.cso", mPd3dDevice, &mPPixelShader);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./12-MultipleRenderTarget/GaussianBlurPS.cso", mPd3dDevice, &mPGaussianBlurPixelShader);
	if( FAILED( result ) )
		return result;

	// Creazione del costant buffer per il filtro gaussiano.
	XMFLOAT4 texelDistance;
	texelDistance = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	texelDistance.x = 1.0f / getWindowWidth();
	texelDistance.y = 1.0f / getWindowHeight();
	result = mcg::createCostantBuffer(mPd3dDevice, &texelDistance, D3D11_USAGE_DEFAULT, &mPixelInfoBuffer);
    if( FAILED( result ) )
        return result;

	// Creazione del costant buffer per le trasformazioni.
	mCamera.translate(-2.0f);
	mTransforms.view = mCamera.getViewMatrix();
	mTransforms.cameraPosition = mCamera.getCameraPositionFlaot4();
	mTransforms.world = XMMatrixScaling(2.0f, 2.0f, 2.0f);
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

	// Creazione del costant buffer per il materiale.
	mMaterial.ambiental = XMFLOAT4(0.1f, 0.3f, 0.3f, 1.0f);
	mMaterial.diffusive = XMFLOAT4(0.2f, 0.5f, 0.5f, 1.0f);
	mMaterial.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mMaterial.shininess = 128;

	result = mcg::createCostantBuffer(mPd3dDevice, &mMaterial, D3D11_USAGE_DEFAULT, &mMaterialCBuffer);
    if( FAILED( result ) )
        return result;

	D3D11_DEPTH_STENCIL_DESC dsDescON;
	dsDescON.DepthEnable = true;
	dsDescON.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescON.DepthFunc = D3D11_COMPARISON_LESS;
	dsDescON.StencilEnable = false;

	result = mPd3dDevice->CreateDepthStencilState(&dsDescON, &mPDepthStencilState);
	if( FAILED( result ) )
        return result;

	//////////////////
	mTextDrawer = new mcg::TextDrawer(mPd3dDeviceContext);
	mArialFont = new mcg::TextFont(mPd3dDevice, L"./Data/arial16.spritefont");
	mTextDrawer->setColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 10.0f));
	//////////////////

	result = initializeFullScreenQuad();
	if(FAILED(result))
		return result;

	return initializeSecondRenderTarget();
}

HRESULT MultipleRenderTarget::initializeSecondRenderTarget()
{
	RECT rc;
    GetClientRect( mHWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

	HRESULT result = S_OK;

	D3D11_TEXTURE2D_DESC textureDesc;
	// Inizializziamo la descrizione delle textures per il render target.
	// Le dimensioni devono essere le stesse della finestra.
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	// Inizializziamo la descrizione per i render targets.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	// Inizializziamo la shader resource view delle textures per gli shaders.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for(unsigned int i = 0; i < 2; ++i)
	{
		// Creiamo le textures.
		result = mPd3dDevice->CreateTexture2D(&textureDesc, NULL, &mSecondRenderTargetTextures[i]);
		if(FAILED(result))
			return result;

		// Creiamo i render target.
		result = mPd3dDevice->CreateRenderTargetView(mSecondRenderTargetTextures[i], &renderTargetViewDesc, &mSecondRenderTargetView[i]);
		if(FAILED(result))
			return result;

		// Creiamo gli shader resource view.
		result = mPd3dDevice->CreateShaderResourceView(mSecondRenderTargetTextures[i], &shaderResourceViewDesc, &mSecondShaderResourceView[i]);
		if(FAILED(result))
			return result;
	}

	return result;
}

HRESULT MultipleRenderTarget::initializeFullScreenQuad()
{
	// Creiamo un quadrato con coordinate texture per il secondo passo di rendering.
	HRESULT result = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
    {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	FullScreenQuadVertex screenQuad[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
	};

	D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof( FullScreenQuadVertex ) * 6;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = screenQuad;

	// Creiamo il vertex buffer nel device.
	result = mPd3dDevice->CreateBuffer( &bufferDesc, &initData, &mPQuadVertexBuffer );
    if( FAILED( result ) )
        return result;

	ID3DBlob* vsBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./12-MultipleRenderTarget/FullScreenQuadVS.cso", mPd3dDevice, &mPQuadVertexShader, &vsBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsBlob, mPd3dDevice, layout, 2, &mPQuadInputLayout);
	if( FAILED( result ) )
        return result;

	vsBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./12-MultipleRenderTarget/FullScreenQuadPS.cso", mPd3dDevice, &mPQuadPixelShader);
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

    result = mPd3dDevice->CreateSamplerState( &sampDesc, &mQuadTexSamplerState );
    if( FAILED( result ) )
        return result;

	return result;
}

void MultipleRenderTarget::render()
{
	renderingPass0();
	renderingPass1();
	// Swap.
	mPSwapChain->Present( 0, 0 );
}

void MultipleRenderTarget::renderingPass0()
{
	// Renderizziamo la scena su 2 textures.

	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f );

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(2, mSecondRenderTargetView, mPDepthStencilView);

	// Pulizia del render target.
	for(unsigned int i = 0; i < 2; ++i)
		mPd3dDeviceContext->ClearRenderTargetView( mSecondRenderTargetView[i], &(clearColor.x) );

	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilState, 0);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPInputLayout);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPVertexShader, nullptr, 0);
	mPd3dDeviceContext->PSSetShader(mPPixelShader, nullptr, 0);
	//Settiamo i constant buffer da utilizzare.
	mPd3dDeviceContext->VSSetConstantBuffers(0, 1, &mTransfCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(0, 1, &mMaterialCBuffer);
	mPd3dDeviceContext->PSSetConstantBuffers(1, 1, &mLightsCBuffer);

	UINT stride = sizeof( mcg::PositionNormal );
    UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers( 0, 1, &mPModelVertexBuffer, &stride, &offset );
	mPd3dDeviceContext->IASetIndexBuffer( mPModelIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	mPd3dDeviceContext->DrawIndexed( mNunberOfModelIndices, 0, 0 );
}
 

void MultipleRenderTarget::renderingPass1()
{
	// Renderizziamo un quadrato a "tutto schermo" con una texture mappata.

	XMFLOAT4 clearColor(0.0f, 0.0f, 0.0f, 1.0f );

	// Settiamo il render target corrente.
	ID3D11RenderTargetView* targets[2] = { mPRenderTargetView, nullptr };
	mPd3dDeviceContext->OMSetRenderTargets(2, targets, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilState, 0);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPQuadInputLayout);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPQuadVertexShader, nullptr, 0);

	if(blurImage)
	{
		mPd3dDeviceContext->PSSetShader(mPGaussianBlurPixelShader, nullptr, 0);
		mPd3dDeviceContext->PSSetConstantBuffers(0, 1, &mPixelInfoBuffer);
	}
	else
		mPd3dDeviceContext->PSSetShader(mPQuadPixelShader, nullptr, 0);

	// Settiamo le texture da utilizzare.
	unsigned int texIndex = (showNormalTexture)? 1 : 0;
	mPd3dDeviceContext->PSSetShaderResources( 0, 1, &mSecondShaderResourceView[texIndex] );
	mPd3dDeviceContext->PSSetSamplers( 0, 1, &mQuadTexSamplerState );

	UINT stride = sizeof( FullScreenQuadVertex );
    UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers( 0, 1, &mPQuadVertexBuffer, &stride, &offset );

	mPd3dDeviceContext->Draw(6, 0);

	/////////////////////////////
	mTextDrawer->beginDraw();
	if(showNormalTexture)
		mTextDrawer->drawText(*mArialFont, L"Normal Texture");
	else
		mTextDrawer->drawText(*mArialFont, L"Color Texture");
	mTextDrawer->endDraw();
	////////////////////////////
}

void MultipleRenderTarget::preRender()
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

void MultipleRenderTarget::keyPressed(WPARAM key, LPARAM param)
{
	switch(key)
	{
		case VK_SPACE: showNormalTexture = !showNormalTexture; break;
		case 0x42: blurImage = !blurImage; break; // B
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

void MultipleRenderTarget::keyReleased(WPARAM key, LPARAM param)
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

void MultipleRenderTarget::cleanResouces()
{
	if(mPModelVertexBuffer)
		mPModelVertexBuffer->Release();

	if(mPModelIndexBuffer)
		mPModelIndexBuffer->Release();

	if(mPVertexShader)
		mPVertexShader->Release();

	if(mPPixelShader)
		mPPixelShader->Release();

	if(mPInputLayout)
		mPInputLayout->Release();

	if(mPDepthStencilState)
		mPDepthStencilState->Release();

	if(mTransfCBuffer)
		mTransfCBuffer->Release();

	if(mLightsCBuffer)
		mLightsCBuffer->Release();

	if(mMaterialCBuffer)
		mMaterialCBuffer->Release();

	if(mPQuadVertexBuffer)
		mPQuadVertexBuffer->Release();

	if(mPQuadVertexShader)
		mPQuadVertexShader->Release();

	if(mPQuadPixelShader)
		mPQuadPixelShader->Release();

	if(mPQuadInputLayout)
		mPQuadInputLayout->Release();

	if(mQuadTexSamplerState)
		mQuadTexSamplerState->Release();

	if(mPixelInfoBuffer)
		mPixelInfoBuffer->Release();

	if(mPGaussianBlurPixelShader)
		mPGaussianBlurPixelShader->Release();

	for(unsigned int i = 0; i < 2; ++i)
	{
		if(mSecondRenderTargetTextures[i])
			mSecondRenderTargetTextures[i]->Release();

		if(mSecondRenderTargetView[i])
			mSecondRenderTargetView[i]->Release();

		if(mSecondShaderResourceView[i])
			mSecondShaderResourceView[i]->Release();
	}

	delete mTextDrawer;
	delete mArialFont;
}
