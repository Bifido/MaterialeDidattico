//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <string>
#include <d3d11.h>

class ReflectiveMeshModel
{
public:
	ReflectiveMeshModel();
	~ReflectiveMeshModel();
	bool initialize(ID3D11Device* device, const std::string& mehsFile);

	void setEnvironmentMap(ID3D11ShaderResourceView* cubeTexture);

	void render(ID3D11DeviceContext* context, ID3D11Buffer* transfBuffer, ID3D11Buffer* lightsBuffer);

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11VertexShader* mEnvironmentMapVS;
	ID3D11PixelShader* mEnvironmentMapPS;
	ID3D11InputLayout* mEnvironmentMapInputLayout;
	ID3D11ShaderResourceView* mCubeTextureRV;
	ID3D11SamplerState* mSamplerState;
	unsigned int mNumberOfIndices;
};

