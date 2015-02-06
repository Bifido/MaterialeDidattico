//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <DirectXMath.h>
#include <d3d11.h>

// Structura contentente un vertice (definito da posizione e coordinata texture)
struct SimpleVertexSkybox
{
    DirectX::XMFLOAT3 Pos;
};

class Skybox
{
public:
	// Inizializza variabili oggetto.
	Skybox();
	// Distruttore; distrugge tutte le risorse allocate.
	~Skybox();
	
	// Carica la skybox da un file texture
	HRESULT initAndLoadCubeTexture(ID3D11Device* d3d11Device, LPCWSTR nTextureFile);
	// Effettua il rendering della skybox
	void render(ID3D11DeviceContext* d3d11DeviceContext, ID3D11Buffer* tranfBuffer);

	ID3D11ShaderResourceView* getCubeTexture() const;

private:
	// Buffer per vertici e indici
	ID3D11Buffer* mVertexBufferCube;
	ID3D11Buffer* mIndexBufferCube;

	// Effetto / Tecnica
	ID3D11VertexShader* mSkyboxVS;
	ID3D11PixelShader* mSkyboxPS;
	
	// Input layout
	ID3D11InputLayout* mVertexLayout;

	// Texture cube map
	ID3D11ShaderResourceView* mCubeTextureRV;
	ID3D11SamplerState* mSamplerState;
};
