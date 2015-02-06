//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "statesProgram.h"
#include "shaderLoader.h"
#include "timer.h"
#include "textDrawer.h"
#include "utils.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DXUtils.lib")

#pragma comment(lib, "dxguid.lib") 
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

StatesProgram::StatesProgram(const HINSTANCE hInstance, const int nCmdShow)
	: SampleProgram(hInstance, nCmdShow)
	, mPVertexBuffer(nullptr)
	, mPIndexBuffer(nullptr)
	, mPTranfBuffer(nullptr)
	, mPVertexShader(nullptr)
	, mPPixelShader(nullptr)
	, mPInputLayout(nullptr)
	, mPDepthStencilStateON(nullptr)
	, mPDepthStencilStateWriteOFF(nullptr)
	, mCurrentRasterizerState(nullptr)
	, mBackFaceCullingState(nullptr)
	, mFrontFaceCullingState(nullptr)
	, mNoFaceCullingState(nullptr)
	, mWireframeState(nullptr)
	, mPPlaneVertexBuffer(nullptr)
	, mPPlaneIndexBuffer(nullptr)
{
	for(int i = 0; i < MAX_KEY; ++i)
		keys[i] = false;
}

void StatesProgram::cleanResouces()
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

	if(mPDepthStencilStateON)
		mPDepthStencilStateON->Release();

	if(mPDepthStencilStateWriteOFF)
		mPDepthStencilStateWriteOFF->Release();

	if(mBackFaceCullingState)
		mBackFaceCullingState->Release();

	if(mFrontFaceCullingState)
		mFrontFaceCullingState->Release();

	if(mNoFaceCullingState)
		mNoFaceCullingState->Release();

	if(mWireframeState)
		mWireframeState->Release();

	if(mPPlaneVertexBuffer)
		mPPlaneVertexBuffer->Release();

	if(mPPlaneIndexBuffer)
		mPPlaneIndexBuffer->Release();
}

HRESULT StatesProgram::initializeResources()
{
	// Creazione del cubo da renderizzare.
	SimpleVertex cube[24] = 
	{
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f) },

		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3( -1.0f, -1.0f, -1.0f), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f) },

		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f) },
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

	SimpleVertex plane[4] = 
	{
		{ XMFLOAT3( -1.0f, -1.0f, 0.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 0.5f) },
		{ XMFLOAT3( 1.0f, -1.0f, 0.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 0.5f) },
		{ XMFLOAT3( 1.0f, 1.0f, 0.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 0.5f) },
		{ XMFLOAT3( -1.0f, 1.0f, 0.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 0.5f) },
	};

	unsigned int planeIndices[] =
    {
        3,1,0,
        2,1,3,
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
    bufferDesc.ByteWidth = sizeof( SimpleVertex ) * 24;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = cube;

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

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof( SimpleVertex ) * 4;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

	initData.pSysMem = plane;

	// Creiamo il vertex buffer nel device.
	result = mPd3dDevice->CreateBuffer( &bufferDesc, &initData, &mPPlaneVertexBuffer );
    if( FAILED( result ) )
        return result;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof( unsigned int ) * 6;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	initData.pSysMem = planeIndices;
	// Creiamo l'index buffer nel device.
	result = mPd3dDevice->CreateBuffer( &bufferDesc, &initData, &mPPlaneIndexBuffer );
	if( FAILED( result ) )
		return result;

	ID3DBlob* vsBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./09-RasterizerStates/vertexShader.cso", mPd3dDevice, &mPVertexShader, &vsBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsBlob, mPd3dDevice, layout, 2, &mPInputLayout);
	if( FAILED( result ) )
        return result;

	vsBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./09-RasterizerStates/pixelShader.cso", mPd3dDevice, &mPPixelShader);
	if( FAILED( result ) )
        return result;

	// Creazione delle matrici di trasformazione
	// World
	mCubeWorldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	mPlaneWorldMatrix = XMMatrixMultiply( XMMatrixTranslation(0.0f, 0.0f, -5.0f), XMMatrixRotationY(XMConvertToRadians(45.0f)) );
	transf.world = mCubeWorldMatrix;

	// View
	transf.view = mCamera.getViewMatrix();
	transf.cameraPosition = mCamera.getCameraPositionFlaot4();

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

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;

	result = mPd3dDevice->CreateDepthStencilState(&dsDesc, &mPDepthStencilStateON);
	if( FAILED( result ) )
        return result;

	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	result = mPd3dDevice->CreateDepthStencilState(&dsDesc, &mPDepthStencilStateWriteOFF);
	if( FAILED( result ) )
        return result;

	D3D11_RASTERIZER_DESC rsDesc;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = true;
	rsDesc.DepthBias = false;
	rsDesc.DepthBiasClamp = 0;
	rsDesc.SlopeScaledDepthBias = 0;
	rsDesc.DepthClipEnable = true;
	rsDesc.ScissorEnable = false;
	rsDesc.MultisampleEnable = false;

	result = mPd3dDevice->CreateRasterizerState(&rsDesc, &mBackFaceCullingState);
	if(FAILED(result))
		return result;

	rsDesc.CullMode = D3D11_CULL_BACK;
	result = mPd3dDevice->CreateRasterizerState(&rsDesc, &mFrontFaceCullingState);
	if(FAILED(result))
		return result;

	rsDesc.CullMode = D3D11_CULL_NONE;
	result = mPd3dDevice->CreateRasterizerState(&rsDesc, &mNoFaceCullingState);
	if(FAILED(result))
		return result;

	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	result = mPd3dDevice->CreateRasterizerState(&rsDesc, &mWireframeState);
	if(FAILED(result))
		return result;

	setCurrentRSState(mBackFaceCullingState, "Back face culling");

	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	result = mPd3dDevice->CreateBlendState(&blendDesc, &mBlendingStateON);
	if(FAILED(result))
		return result;

	blendDesc.RenderTarget[0].BlendEnable = false;
	result = mPd3dDevice->CreateBlendState(&blendDesc, &mBlendingStateOFF);
	if(FAILED(result))
		return result;

	setCurrentBlendState( mBlendingStateOFF, "Alpha Blending OFF");

	//////////////////
	mTextDrawer = new mcg::TextDrawer(mPd3dDeviceContext);
	mArialFont = new mcg::TextFont(mPd3dDevice, L"./Data/arial16.spritefont");
	mTextDrawer->setColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	//////////////////

	return result;
}

void StatesProgram::render()
{
	XMFLOAT4 clearColor(0.39f, 0.58f, 0.93f, 1.0f );

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, &(clearColor.x) );
	// Pulizia del depth buffer,
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPInputLayout);
	// Settiamo il vertex buffer da renderizzare,
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mPVertexBuffer, &stride, &offset);
	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Settiamo gli shader da utilizzare.
	mPd3dDeviceContext->VSSetShader(mPVertexShader, nullptr, 0);
    mPd3dDeviceContext->PSSetShader(mPPixelShader, nullptr, 0);

	//Settiamo i constant buffer da utilizzare.
	mPd3dDeviceContext->VSSetConstantBuffers(0, 1, &mPTranfBuffer);

	mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilStateON, 0);

	// Settiamo il rasterizer state.
	mPd3dDeviceContext->RSSetState(mCurrentRasterizerState);

	transf.world = mCubeWorldMatrix;
	mPd3dDeviceContext->UpdateSubresource( mPTranfBuffer, 0, nullptr, &transf, 0, 0 );

	mPd3dDeviceContext->IASetIndexBuffer(mPIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(36, 0, 0);

	mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilStateWriteOFF, 0);
	mPd3dDeviceContext->OMSetBlendState(mCurrentBlendState, nullptr, 0xffffffff);
	transf.world = mPlaneWorldMatrix;
	mPd3dDeviceContext->UpdateSubresource( mPTranfBuffer, 0, nullptr, &transf, 0, 0 );
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mPPlaneVertexBuffer, &stride, &offset);
	mPd3dDeviceContext->IASetIndexBuffer(mPPlaneIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(6, 0, 0);

	/////////////////////////////
	mTextDrawer->beginDraw();
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 10.0f));
	mTextDrawer->drawText(*mArialFont, mcg::toWString(mCurrentRSName));
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 40.0f));
	mTextDrawer->drawText(*mArialFont, mcg::toWString(mCurrentBSName));
	mTextDrawer->endDraw();
	////////////////////////////

	// Swap.
	mPSwapChain->Present( 0, 0 );
}

void StatesProgram::keyPressed(WPARAM key, LPARAM param)
{
	switch(key)
	{
	case 0x41: keys[ROTATE_CAMERA_LEFT] = true; break; // A
	case 0x44: keys[ROTATE_CAMERA_RIGHT] = true; break; // D
	case 0x57: keys[ROTATE_CAMERA_UP] = true; break; // W
	case 0x53: keys[ROTATE_CAMERA_DOWN] = true; break; // S
	case 0x51: keys[MOVE_CAMERA_FOREWARD] = true; break; // Q
	case 0x45: keys[MOVE_CAMERA_BACKWARD] = true; break; // E
	case VK_LEFT: keys[ROTATE_OBJECT_LEFT] = true; break;
	case VK_RIGHT: keys[ROTATE_OBJECT_RIGHT] = true; break;
	case VK_UP: keys[ROTATE_OBJECT_UP] = true; break;
	case VK_DOWN: keys[ROTATE_OBJECT_DOWN] = true; break;
	// NUMBERS
	case 0x31: setCurrentRSState(mBackFaceCullingState, "Back face culling"); break;
	case 0x32: setCurrentRSState(mFrontFaceCullingState, "Front face culling"); break;
	case 0x33: setCurrentRSState(mNoFaceCullingState, "No face culling"); break;
	case 0x34: setCurrentRSState(mWireframeState, "Wireframe"); break;
	case 0x35: setCurrentBlendState( mBlendingStateOFF, "Alpha Blending OFF"); break;
	case 0x36: setCurrentBlendState( mBlendingStateON, "Alpha Blending ON"); break;
	default: SampleProgram::keyPressed(key, param);
	}
}

void StatesProgram::keyReleased(WPARAM key, LPARAM param)
{
	switch(key)
	{
	case 0x41: keys[ROTATE_CAMERA_LEFT] = false; break; // A
	case 0x44: keys[ROTATE_CAMERA_RIGHT] = false; break; // D
	case 0x57: keys[ROTATE_CAMERA_UP] = false; break; // W
	case 0x53: keys[ROTATE_CAMERA_DOWN] = false; break; // S
	case 0x51: keys[MOVE_CAMERA_FOREWARD] = false; break; // Q
	case 0x45: keys[MOVE_CAMERA_BACKWARD] = false; break; // E
	case VK_LEFT: keys[ROTATE_OBJECT_LEFT] = false; break;
	case VK_RIGHT: keys[ROTATE_OBJECT_RIGHT] = false; break;
	case VK_UP: keys[ROTATE_OBJECT_UP] = false; break;
	case VK_DOWN: keys[ROTATE_OBJECT_DOWN] = false; break;
	default: SampleProgram::keyReleased(key, param);
	}
}

void StatesProgram::setCurrentRSState(ID3D11RasterizerState* currentState, const std::string& stateName)
{
	mCurrentRasterizerState = currentState;
	mCurrentRSName = stateName;
}

void StatesProgram::setCurrentBlendState(ID3D11BlendState* state, const std::string& stateName)
{
	mCurrentBlendState = state;
	mCurrentBSName = stateName;
}

void StatesProgram::preRender()
{
	static mcg::Timer timer;
	const float degForSec = 1.0f;

	float fSec = timer.elapsedSecF();

	// Aggiorniamo la mCamera.
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

	XMFLOAT4 axeRotation(0.0f, 0.0f, 0.0f, 0.0f);

	if(keys[ROTATE_OBJECT_RIGHT])
		axeRotation.y -= 1.0f;
	if(keys[ROTATE_OBJECT_LEFT])
		axeRotation.y += 1.0f;
	if(keys[ROTATE_OBJECT_UP])
		axeRotation.x -= 1.0f;
	if(keys[ROTATE_OBJECT_DOWN])
		axeRotation.x += 1.0f;

	if(axeRotation.x != 0.0f || axeRotation.y != 0.0f)
	{
		float angle = degForSec * fSec;

		XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat4(&axeRotation), angle);
		XMMATRIX newWorldMatrix = XMMatrixMultiply(rotationMatrix, mCubeWorldMatrix);
		mCubeWorldMatrix = newWorldMatrix;
	}

	transf.view = mCamera.getViewMatrix();
	transf.cameraPosition = mCamera.getCameraPositionFlaot4();

	timer.start();
}

