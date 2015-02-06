//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "multisampleProgram.h"
#include "shaderLoader.h"
#include "timer.h"

#pragma comment(lib, "DXUtils.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

MultisampleProgram::MultisampleProgram(const HINSTANCE hInstance, const int nCmdShow)
	: SampleProgram(hInstance, nCmdShow)
	, mPVertexBuffer(nullptr)
	, mPIndexBuffer(nullptr)
	, mPTranfBuffer(nullptr)
	, mPVertexShader(nullptr)
	, mPPixelShader(nullptr)
	, mPInputLayout(nullptr)
	, mPDepthStencilState(nullptr)

{
	for(int i = 0; i < 10; ++i)
		keyState[i] = false;
}

void MultisampleProgram::cleanResouces()
{
	if(mPVertexBuffer)
		mPVertexBuffer->Release();

	if(mPIndexBuffer)
		mPIndexBuffer->Release();

	if(mPTranfBuffer)
		mPTranfBuffer->Release();

	if(mPVertexShader)
		mPVertexShader->Release();

	if(mPPixelShader)
		mPPixelShader->Release();

	if(mPInputLayout)
		mPInputLayout->Release();

	if(mPDepthStencilState)
		mPDepthStencilState->Release();
}

HRESULT MultisampleProgram::initializeResources()
{
	// Creazione del cubo da renderizzare.
	SimpleVertex vertices[] =
	{
		{ XMFLOAT3( -1.0f,  1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3(  1.0f,  1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3(  1.0f,  1.0f,  1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f,  1.0f,  1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3(  1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3(  1.0f, -1.0f,  1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f,  1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
	};

	unsigned int indices[] =
	{
		3,1,0,
		2,1,3, // Prima faccia cubo

		0,5,4,
		1,5,0, // Seconda faccia cubo

		3,4,7,
		0,4,3, // Terza faccia cubo

		1,6,5,
		2,6,1, // Quarta faccia cubo

		2,7,6,
		3,7,2, // Quinta faccia cubo

		6,4,5,
		7,4,6, // Sesta faccia cubo
	};

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	HRESULT result = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof( SimpleVertex ) * 8;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = vertices;

	// Creiamo il vertex buffer nel device.
	result = mPd3dDevice->CreateBuffer( &bufferDesc, &initData, &mPVertexBuffer );
    if( FAILED( result ) )
        return result;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( unsigned int ) * 36;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	initData.pSysMem = indices;
	// Creiamo l'index buffer nel device.
	result = mPd3dDevice->CreateBuffer( &bufferDesc, &initData, &mPIndexBuffer );
	if( FAILED( result ) )
		return result;

	ID3DBlob* vsBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./03-Transformations/vertexShader.cso", mPd3dDevice, &mPVertexShader, &vsBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsBlob, mPd3dDevice, layout, 2, &mPInputLayout);
	if( FAILED( result ) )
        return result;

	vsBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./03-Transformations/pixelShader.cso", mPd3dDevice, &mPPixelShader);
	if( FAILED( result ) )
        return result;

	// Creazione delle matrici di trasformazione
	// World
	transf.world = XMMatrixScaling(2.0f, 2.0f, 2.0f);

	// View
	transf.view = camera.getViewMatrix();
	transf.cameraPosition = camera.getCameraPositionFlaot4();

	// Projection
	transf.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), getWindowAspectRatio(), 0.1f, 100.0f);

	// Creiamo un constant buffer per passare le matrici agli shaders (al vertex shader).
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(mcg::Transformations);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	result = mPd3dDevice->CreateBuffer( &bufferDesc, nullptr, &mPTranfBuffer );
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

	return result;
}

void MultisampleProgram::render()
{
	// Aggiorniamo il constant buffer delle trasformazioni
	mPd3dDeviceContext->UpdateSubresource( mPTranfBuffer, 0, nullptr, &transf, 0, 0 );

	XMFLOAT4 clearColor(0.39f, 0.58f, 0.93f, 1.0f );

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPInputLayout);
	// Settiamo il vertex buffer da renderizzare
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mPVertexBuffer, &stride, &offset);
	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPVertexShader, nullptr, 0);
    mPd3dDeviceContext->PSSetShader(mPPixelShader, nullptr, 0);

	//Settiamo i constant buffer da utilizzare.
	mPd3dDeviceContext->VSSetConstantBuffers(0, 1, &mPTranfBuffer);

	mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilState, 0);

	mPd3dDeviceContext->IASetIndexBuffer(mPIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(36, 0, 0);

	// Swap.
	mPSwapChain->Present( 0, 0 );
}

void MultisampleProgram::keyPressed(WPARAM key, LPARAM param)
{
	switch(key)
	{
	case 0x57: keyState[0] = true; break; // W
	case 0x41: keyState[1] = true; break; // A
	case 0x53: keyState[2] = true; break; // S
	case 0x44: keyState[3] = true; break; // D
	case VK_LEFT: keyState[4] = true; break;
	case VK_RIGHT: keyState[5] = true; break;
	case VK_UP: keyState[6] = true; break;
	case VK_DOWN: keyState[7] = true; break;
	case 0x51: keyState[8] = true; break; // Q
	case 0x45: keyState[9] = true; break; // E
	default: SampleProgram::keyPressed(key, param);
	}
}

void MultisampleProgram::keyReleased(WPARAM key, LPARAM param)
{
	switch(key)
	{
	case 0x57: keyState[0] = false; break; // W
	case 0x41: keyState[1] = false; break; // A
	case 0x53: keyState[2] = false; break; // S
	case 0x44: keyState[3] = false; break; // D
	case VK_LEFT: keyState[4] = false; break;
	case VK_RIGHT: keyState[5] = false; break;
	case VK_UP: keyState[6] = false; break;
	case VK_DOWN: keyState[7] = false; break;
	case 0x51: keyState[8] = false; break; // Q
	case 0x45: keyState[9] = false; break; // E
	default: SampleProgram::keyReleased(key, param);
	}
}

void MultisampleProgram::preRender()
{
	static mcg::Timer timer;
	const float degForSec = 1.0f;

	float fSec = timer.elapsedSecF();
	XMFLOAT4 viewAxeRotation(0.0f, 0.0f, 0.0f, 0.0f);
	float viewTraslation = 0.0f;

	if(keyState[0])
		camera.xRotation(-fSec);
	if(keyState[1])
		camera.yRotation(fSec);
	if(keyState[2])
		camera.xRotation(fSec);
	if(keyState[3])
		camera.yRotation(-fSec);

	if(keyState[8])
		viewTraslation += (2.0f * fSec);
	if(keyState[9])
		viewTraslation -= (2.0f * fSec);

	if(viewTraslation != 0.0f)
		camera.translate(viewTraslation);	

	XMFLOAT4 axeRotation(0.0f, 0.0f, 0.0f, 0.0f);

	if(keyState[4])
		axeRotation.y -= 1.0f;
	if(keyState[5])
		axeRotation.y += 1.0f;
	if(keyState[6])
		axeRotation.x -= 1.0f;
	if(keyState[7])
		axeRotation.x += 1.0f;

	if(axeRotation.x != 0.0f || axeRotation.y != 0.0f)
	{
		float angle = degForSec * fSec;

		XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&axeRotation), angle);
		XMMATRIX newWorldMatrix = XMMatrixMultiply(rotationMatrix, transf.world);
		transf.world = newWorldMatrix;
	}

	transf.view = camera.getViewMatrix();
	transf.cameraPosition = camera.getCameraPositionFlaot4();

	timer.start();
}

