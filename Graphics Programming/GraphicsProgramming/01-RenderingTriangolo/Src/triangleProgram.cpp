//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "triangleProgram.h"
#include "types.h"
#include "utils.h"

#include <d3dcompiler.h>


TriangleProgram::TriangleProgram(const HINSTANCE hInstance, const int nCmdShow)
	: mcg::SampleProgram(hInstance, nCmdShow)
	, mPVertexBuffer(nullptr)
	, mPVertexShader(nullptr)
	, mPPixelShader(nullptr)
	, mPInputLayout(nullptr)
{
}

void TriangleProgram::cleanResouces()
{
	if(mPVertexBuffer)
		mPVertexBuffer->Release();

	if(mPVertexShader)
		mPVertexShader->Release();

	if(mPPixelShader)
		mPPixelShader->Release();

	if(mPInputLayout)
		mPInputLayout->Release();
}

HRESULT TriangleProgram::initializeResources()
{
	// Vertici del triangolo da renderizzare.
	unsigned int vercicesNum = 3;
	DirectX::XMFLOAT3 vertices[] =
	{
		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),
		DirectX::XMFLOAT3(0.0f,  0.5f, 0.5f),
		DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),
	};

	// Descrizione del buffer.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * vercicesNum;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices;

	// Creazione del buffer.
	HRESULT result = mPd3dDevice->CreateBuffer(&bufferDesc, &initData, &mPVertexBuffer);
	if(FAILED(result))
	{
		mcg::showErrorMessageBox(L"Vertex buffer creation failed!", L"Error", mHWnd);
		return E_FAIL;
	}

	// Descrizione del layout del buffer.
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		"POSITION",
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0,
	};

	// Creazione degli shaders.
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

	// Vertex shader (compilato a build-time).
	// Caricamento del precompilato.
    ID3DBlob* vertexShaderBlob = nullptr;
	result = D3DReadFileToBlob(L"./01-RenderingTriangolo/vertexShader.cso", &vertexShaderBlob);
	if(FAILED(result))
	{
		mcg::showErrorMessageBox(L"Vertex shader loading failed.", L"Error", mHWnd);
		return E_FAIL;
	}

	// Creazione dello shader.
	result = mPd3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &mPVertexShader);
	if(FAILED(result))
	{
		mcg::showErrorMessageBox(L"Vertex shader creation failed.", L"Error", mHWnd);
		return E_FAIL;
	}

	// Pixel shader (compilazione a run-time, necessita di d3dcompiler.lib e d3dcompiler.h).
	ID3DBlob* pixelShaderBlob = nullptr;
    ID3DBlob* pixelShaderErrorBlob = nullptr;

	// Compilazione.
    result = D3DCompileFromFile( L"./01-RenderingTriangolo/pixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                     "SimplePixelShader", "ps_4_0_level_9_3",
                                     flags, 0, &pixelShaderBlob, &pixelShaderErrorBlob );
    if (FAILED(result))
    {
        if ( pixelShaderErrorBlob ) // Stampiamo l'errore.
        {
			std::wstring wstr = mcg::toWString((char*)pixelShaderErrorBlob->GetBufferPointer());
			mcg::showErrorMessageBox(wstr.data(), L"Pixel shader compiling.", mHWnd);
            pixelShaderErrorBlob->Release();
        }

        if ( pixelShaderBlob )
           pixelShaderBlob->Release();

		return E_FAIL;
	}

	// Creazione dello shader.
	result = mPd3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &mPPixelShader);
	if(FAILED(result))
	{
		mcg::showErrorMessageBox(L"Pixel shader creation failed.", L"Error", mHWnd);
		return E_FAIL;
	}

	// Creazione dell'input layout per il vertex shader.
	result = mPd3dDevice->CreateInputLayout(layoutDesc, 1, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &mPInputLayout);
	if ( FAILED(result) )
    {
		mcg::showErrorMessageBox(L"Input layout creation failed!", L"Error", mHWnd);
		return E_FAIL;
	}

	// Pulizia.
    if(vertexShaderBlob)
        vertexShaderBlob->Release();

    if(pixelShaderBlob)
        pixelShaderBlob->Release();

    if(pixelShaderErrorBlob)
        pixelShaderErrorBlob->Release();

	return S_OK;
}

void TriangleProgram::render()
{
	FLOAT clearColor[] = {0.39f, 0.58f, 0.93f, 1.0f};

	// Settiamo il render target corrente.
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, nullptr);
	// Pullizia del render target.
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, clearColor );
	// Settiamo l'input layout.
	mPd3dDeviceContext->IASetInputLayout(mPInputLayout);
	// Settiamo il vertex buffer da renderizzare
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	mPd3dDeviceContext->IASetVertexBuffers(0, 1, &mPVertexBuffer, &stride, &offset);
	// Settiamo la topologia della nostra geometria.
	mPd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Settiamo gli shader da utilizzare
	mPd3dDeviceContext->VSSetShader(mPVertexShader, nullptr, 0);
    mPd3dDeviceContext->PSSetShader(mPPixelShader, nullptr, 0);

	// Disegno del vertex buffer.
	mPd3dDeviceContext->Draw(3, 0);

	// Swap.
	mPSwapChain->Present( 0, 0 );
}
