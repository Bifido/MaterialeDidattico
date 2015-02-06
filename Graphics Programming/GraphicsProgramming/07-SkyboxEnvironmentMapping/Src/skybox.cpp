//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "skybox.h"
#include "DDSTextureLoader.h"
#include "shaderLoader.h"

// Costruttore
Skybox::Skybox()
	: mVertexBufferCube(nullptr)
	, mIndexBufferCube(nullptr)
	, mVertexLayout(nullptr)
	, mCubeTextureRV(nullptr)
	, mSkyboxVS(nullptr)
	, mSkyboxPS(nullptr)
	, mSamplerState(nullptr)
{}


// Distruttore
Skybox::~Skybox()
{
	if(mVertexBufferCube)
		mVertexBufferCube->Release();

	if(mIndexBufferCube)
		mIndexBufferCube->Release();

	if(mVertexLayout)
		mVertexLayout->Release();

	if(mCubeTextureRV)
		mCubeTextureRV->Release();

	if(mCubeTextureRV)
		mSkyboxVS->Release();

	if(mSkyboxPS)
		mSkyboxPS->Release();

	if(mSamplerState)
		mSamplerState->Release();
}

// Carica la skybox da un file texture
HRESULT Skybox::initAndLoadCubeTexture(ID3D11Device* d3d11Device, LPCWSTR nTextureFile)
{
	HRESULT result = S_OK;
	// Definiamo il cubo
	// Create vertex buffer Posizione - Normale - Coordinate texture
	SimpleVertexSkybox vertices[] =
	{
		{ DirectX::XMFLOAT3( -1.0f,  1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f,  1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f,  1.0f,  1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f,  1.0f,  1.0f ) },

		{ DirectX::XMFLOAT3( -1.0f, -1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f, -1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f, -1.0f,  1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f, -1.0f,  1.0f ) },

		{ DirectX::XMFLOAT3( -1.0f, -1.0f,  1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f, -1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f,  1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f,  1.0f,  1.0f ) },

		{ DirectX::XMFLOAT3( 1.0f, -1.0f,  1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f, -1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f,  1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3( 1.0f,  1.0f,  1.0f ) },

		{ DirectX::XMFLOAT3( -1.0f, -1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f, -1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f,  1.0f, -1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f,  1.0f, -1.0f ) },

		{ DirectX::XMFLOAT3( -1.0f, -1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f, -1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3(  1.0f,  1.0f, 1.0f ) },
		{ DirectX::XMFLOAT3( -1.0f,  1.0f, 1.0f ) },
	};

	// Creazione vertex buffer
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof( SimpleVertexSkybox ) * 24;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = vertices;

    result = d3d11Device->CreateBuffer( &bufferDesc, &initData, &mVertexBufferCube );
    if( FAILED( result ) )
        return result;

	// Index buffer
    unsigned int indices[] =
    {
        3,0,1, // Prima faccia
        2,3,1,

        6,5,4, // Seconda faccia
        7,6,4,

        11,8,9, // Terza faccia
        10,11,9,

        14,13,12, // Quarta faccia
        15,14,12,

        19,16,17, // Quinta faccia
        18,19,17,

        22,21,20, // Sesta faccia
        23,22,20
    };

	// Creazione index buffer
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(  unsigned int ) * 36;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    initData.pSysMem = indices;

    result = d3d11Device->CreateBuffer( &bufferDesc, &initData, &mIndexBufferCube );
    if( FAILED( result ) )
        return result;

	// Carichiamo la texture
	result = DirectX::CreateDDSTextureFromFile(d3d11Device, nTextureFile, nullptr, &mCubeTextureRV);
	if( FAILED( result ) )
        return result;

	// Definiamo l'input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
    unsigned int numElements = sizeof( layout ) / sizeof( layout[0] );

	ID3DBlob* vsBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./07-SkyboxEnvironmentMapping/SkyboxVS.cso", d3d11Device, &mSkyboxVS, &vsBlob);
	if( FAILED( result ) )
        return result;

	result = mcg::ShaderLoader::createInputLayout(vsBlob, d3d11Device, layout, 1, &mVertexLayout);
	if( FAILED( result ) )
        return result;

	vsBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./07-SkyboxEnvironmentMapping/SkyboxPS.cso", d3d11Device, &mSkyboxPS);
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

    result = d3d11Device->CreateSamplerState( &sampDesc, &mSamplerState );
    if( FAILED( result ) )
        return result;

	return result;
}

// Effettua il rendering della skybox
void Skybox::render(ID3D11DeviceContext* d3d11DeviceContext, ID3D11Buffer* tranfBuffer)
{
	// Sanity check
	if(mCubeTextureRV == nullptr || mVertexLayout == nullptr || mVertexBufferCube == nullptr || mIndexBufferCube == nullptr)
		return;
    // Rendering mesh cubo
    d3d11DeviceContext->IASetInputLayout( mVertexLayout );
    // Setta primitiva
    d3d11DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	d3d11DeviceContext->VSSetShader(mSkyboxVS, nullptr, 0);
	d3d11DeviceContext->PSSetShader(mSkyboxPS, nullptr, 0);
	d3d11DeviceContext->VSSetConstantBuffers(0, 1, &tranfBuffer);

	d3d11DeviceContext->PSSetShaderResources( 0, 1, &mCubeTextureRV );

	d3d11DeviceContext->PSSetSamplers( 0, 1, &mSamplerState );

	// Settiamo i buffer
	UINT stride = sizeof( SimpleVertexSkybox );
    UINT offset = 0;
    d3d11DeviceContext->IASetVertexBuffers( 0, 1, &mVertexBufferCube, &stride, &offset );
	d3d11DeviceContext->IASetIndexBuffer( mIndexBufferCube, DXGI_FORMAT_R32_UINT, 0 );

	d3d11DeviceContext->DrawIndexed( 36, 0, 0 );
}

ID3D11ShaderResourceView* Skybox::getCubeTexture() const
{
	return mCubeTextureRV;
}
