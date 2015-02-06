//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "zBufferProgram.h"
#include "types.h"
#include "shaderLoader.h"

#include "textDrawer.h"

using namespace DirectX;

ZBufferProgram::ZBufferProgram(const HINSTANCE hInstance, const int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mPVertexBuffer(nullptr)
	, mPIndexBuffer00(nullptr)
	, mPIndexBuffer01(nullptr)
	, mPVertexShader(nullptr)
	, mPPixelShader(nullptr)
	, mPInputLayout(nullptr)
	, mPDepthStencilStateON(nullptr)
	, mPDepthStencilStateOFF(nullptr)
	, mZBufferEnabled(false)
{
}

ZBufferProgram::~ZBufferProgram()
{}

void ZBufferProgram::cleanResouces()
{
	if(mPVertexBuffer)
		mPVertexBuffer->Release();

	if(mPIndexBuffer00)
		mPIndexBuffer00->Release();

	if(mPIndexBuffer01)
		mPIndexBuffer01->Release();

	if(mPVertexShader)
		mPVertexShader->Release();

	if(mPPixelShader)
		mPPixelShader->Release();

	if(mPInputLayout)
		mPInputLayout->Release();

	if(mPDepthStencilStateON)
		mPDepthStencilStateON->Release();
	
	if(mPDepthStencilStateOFF)
		mPDepthStencilStateOFF->Release();

	delete mTextDrawer;
	delete mArialFont;
}

HRESULT ZBufferProgram::initializeResources()
{
	struct VerticesAndColors
	{
		XMFLOAT3 vert;
		XMFLOAT4 col;
	};

	VerticesAndColors verticesAncColors[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, 0.3f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.6f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.5f, 0.3f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.6f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.3f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, -0.5f, 0.6f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
	};

	unsigned int indicesTriangle00[] =
	{
		0, 2, 4,
	};

	unsigned int indicesTriangle01[] =
	{
		1, 3, 5,
	};

	// Settiamo il descrittore per il buffer
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof( VerticesAndColors ) * 6;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Carichiamo la risorsa D3D10_SUBRESOURCE_DATA 
    D3D11_SUBRESOURCE_DATA verticesInitData;
	verticesInitData.pSysMem = verticesAncColors;
	verticesInitData.SysMemPitch = 0;
	verticesInitData.SysMemSlicePitch = 0;

	HRESULT result = mPd3dDevice->CreateBuffer( &vertexBufferDesc, &verticesInitData, &mPVertexBuffer );
    if( FAILED( result ) )
	{
        return result;
	}

	D3D11_BUFFER_DESC indexBufferDesc00;
	indexBufferDesc00.ByteWidth = sizeof(unsigned int) * 3;
	indexBufferDesc00.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc00.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc00.CPUAccessFlags = 0;
	indexBufferDesc00.MiscFlags = 0;
	indexBufferDesc00.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData00;
	indexBufferData00.pSysMem = indicesTriangle00;

	result = mPd3dDevice->CreateBuffer( &indexBufferDesc00, &indexBufferData00, &mPIndexBuffer00 );
    if( FAILED( result ) )
	{
        return result;
	}

	D3D11_BUFFER_DESC indexBufferDesc01;
	indexBufferDesc01.ByteWidth = sizeof(unsigned int) * 3;
	indexBufferDesc01.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc01.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc01.CPUAccessFlags = 0;
	indexBufferDesc01.MiscFlags = 0;
	indexBufferDesc01.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData01;
	indexBufferData01.pSysMem = indicesTriangle01;

	result = mPd3dDevice->CreateBuffer( &indexBufferDesc01, &indexBufferData01, &mPIndexBuffer01 );
    if( FAILED( result ) )
	{
        return result;
	}

	ID3DBlob* vsBlob;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./02-ZBuffer/vertexShader.cso", mPd3dDevice, &mPVertexShader, &vsBlob);
	if( FAILED( result ) )
	{
        return result;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	result = mcg::ShaderLoader::createInputLayout(vsBlob, mPd3dDevice, layout, 2, &mPInputLayout);
	if( FAILED( result ) )
	{
        return result;
	}

	vsBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./02-ZBuffer/pixelShader.cso", mPd3dDevice, &mPPixelShader);
	if( FAILED( result ) )
	{
        return result;
	}

	// Creazione dei DepthStencil state.
	D3D11_DEPTH_STENCIL_DESC dsDescON;
	dsDescON.DepthEnable = true;
	dsDescON.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescON.DepthFunc = D3D11_COMPARISON_LESS;
	dsDescON.StencilEnable = false;

	result = mPd3dDevice->CreateDepthStencilState(&dsDescON, &mPDepthStencilStateON);
	if( FAILED( result ) )
	{
        return result;
	}

	// Creazione del DepthStencil state.
	D3D11_DEPTH_STENCIL_DESC dsDescOFF;
	dsDescOFF.DepthEnable = false;
	dsDescOFF.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDescOFF.DepthFunc = D3D11_COMPARISON_LESS;
	dsDescOFF.StencilEnable = false;

	result = mPd3dDevice->CreateDepthStencilState(&dsDescOFF, &mPDepthStencilStateOFF);
	if( FAILED( result ) )
	{
        return result;
	}

	mTextDrawer = new mcg::TextDrawer(mPd3dDeviceContext);
	mArialFont = new mcg::TextFont(mPd3dDevice, L"./Data/arial16.spritefont");
	mTextDrawer->setColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	mTextDrawer->setPosition(DirectX::XMFLOAT2(10.0f, 10.0f));

	return S_OK;
}

void ZBufferProgram::render()
{
	FLOAT clearColor[] = {0.39f, 0.58f, 0.93f, 1.0f};

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView);
	// Pulizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, clearColor );
	// Pulizia del depth buffer
	mPd3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPInputLayout);
	// Settiamo il vertex buffer da renderizzare
	UINT stride = 28;
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mPVertexBuffer, &stride, &offset);
	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPVertexShader, nullptr, 0);
    mPd3dDeviceContext->PSSetShader(mPPixelShader, nullptr, 0);

	// Abilito o meno lo z-buffer.
	if(mZBufferEnabled)
		mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilStateON, 0);
	else
		mPd3dDeviceContext->OMSetDepthStencilState(mPDepthStencilStateOFF, 0);

	// Disegno del primo triangolo.
	mPd3dDeviceContext->IASetIndexBuffer(mPIndexBuffer00, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(3, 0, 0);

	// Disegno del secondo triangolo.
	mPd3dDeviceContext->IASetIndexBuffer(mPIndexBuffer01, DXGI_FORMAT_R32_UINT, 0);
	mPd3dDeviceContext->DrawIndexed(3, 0, 0);

	// Draw text message
	mTextDrawer->beginDraw();
	if(mZBufferEnabled)
		mTextDrawer->drawText(*mArialFont, L"Z-Buffer ON");
	else
		mTextDrawer->drawText(*mArialFont, L"Z-Buffer OFF");
	mTextDrawer->endDraw();

	// Swap.
	mPSwapChain->Present( 0, 0 );
}

void ZBufferProgram::keyPressed(WPARAM key, LPARAM param)
{
	if(key == VK_SPACE)
		mZBufferEnabled = !mZBufferEnabled;

	SampleProgram::keyPressed(key, param);
}


