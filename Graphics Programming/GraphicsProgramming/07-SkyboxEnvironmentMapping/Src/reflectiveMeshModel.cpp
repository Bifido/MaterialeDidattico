//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "reflectiveMeshModel.h"
#include "stlMeshLoader.h"
#include "shaderLoader.h"

ReflectiveMeshModel::ReflectiveMeshModel()
	: mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mEnvironmentMapVS(nullptr)
	, mEnvironmentMapPS(nullptr)
	, mEnvironmentMapInputLayout(nullptr)
	, mCubeTextureRV(nullptr)
	, mSamplerState(nullptr)
{}

ReflectiveMeshModel::~ReflectiveMeshModel()
{
	if(mVertexBuffer)
		mVertexBuffer->Release();

	if(mIndexBuffer)
		mIndexBuffer->Release();

	if(mEnvironmentMapVS)
		mEnvironmentMapVS->Release();

	if(mEnvironmentMapPS)
		mEnvironmentMapPS->Release();

	if(mEnvironmentMapInputLayout)
		mEnvironmentMapInputLayout->Release();

	if(mCubeTextureRV)
		mCubeTextureRV->Release();

	if(mSamplerState)
		mSamplerState->Release();
}

void ReflectiveMeshModel::setEnvironmentMap(ID3D11ShaderResourceView* cubeTexture)
{
	if(mCubeTextureRV)
		mCubeTextureRV->Release();

	mCubeTextureRV = cubeTexture;
	mCubeTextureRV->AddRef();
}

bool ReflectiveMeshModel::initialize(ID3D11Device* device, const std::string& meshFileName)
{
	// Carichiamo la mesh
	bool ret = true; 
	HRESULT result = S_OK;
	mcg::STLMeshLoader model;
	ret = model.loadFormFile(meshFileName);
	if(!ret)
		return ret;

	// Le mesh stl sono right-hand.
	model.swapHandness();
	// Smoothing delle normali (stl usa normali per faccia.
	// In questo modo ogni vertice avra' una normale continua con tutte le facce vicine).
	model.smoothNormals(25.0f);

	ret = model.getVertexBuffer(device, &mVertexBuffer);
	if(!ret)
		return ret;

	ret = model.getIndexBuffer(device, &mIndexBuffer);
	if(!ret)
		return ret;

	mNumberOfIndices = model.getNumberOfIndices();

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Carichiamo gli shaders
	ID3DBlob* vsEnvBlob = nullptr;
	result = mcg::ShaderLoader::vertexPreBuiltLoad(L"./07-SkyboxEnvironmentMapping/EnvironmentMapVS.cso", device, &mEnvironmentMapVS, &vsEnvBlob);
	if( FAILED( result ) )
        return false;

	result = mcg::ShaderLoader::createInputLayout(vsEnvBlob, device, layout, 2, &mEnvironmentMapInputLayout);
	if( FAILED( result ) )
        return false;

	vsEnvBlob->Release();

	result = mcg::ShaderLoader::pixelPreBuiltLoad(L"./07-SkyboxEnvironmentMapping/EnvironmentMapPS.cso", device, &mEnvironmentMapPS);
	if( FAILED( result ) )
        return false;

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

    result = device->CreateSamplerState( &sampDesc, &mSamplerState );
    if( FAILED( result ) )
        return false;

	return ret;
}

void ReflectiveMeshModel::render(ID3D11DeviceContext* context, ID3D11Buffer* transfBuffer, ID3D11Buffer* lightsBuffer)
{
	context->IASetInputLayout( mEnvironmentMapInputLayout );
    // Setta primitiva
    context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	context->VSSetShader(mEnvironmentMapVS, nullptr, 0);
	context->PSSetShader(mEnvironmentMapPS, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &transfBuffer);
	context->PSSetConstantBuffers(0, 1, &lightsBuffer);

	context->PSSetShaderResources( 0, 1, &mCubeTextureRV );
	context->PSSetSamplers( 0, 1, &mSamplerState );

	// Settiamo i buffer
	UINT stride = sizeof( mcg::PositionNormal );
    UINT offset = 0;
	context->IASetVertexBuffers( 0, 1, &mVertexBuffer, &stride, &offset );
	context->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	context->DrawIndexed( mNumberOfIndices, 0, 0 );
}
