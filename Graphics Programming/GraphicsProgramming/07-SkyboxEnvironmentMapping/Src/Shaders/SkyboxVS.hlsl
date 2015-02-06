//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

cbuffer TransformsBuffer : register(cb0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float3 CameraPosition;
};

struct VertexShaderInput
{
	float3 pos	: POSITION;
};

struct PixelShaderInput
{
    float4 pos	: SV_POSITION;
	float3 tex	: TEXCOORD0;
};

PixelShaderInput SkyboxVS(VertexShaderInput input)
{
	PixelShaderInput output;

    // calcola matrice View e Projection al vertice
    output.pos = mul( View, float4(input.pos.xyz,0) );
    output.pos = mul( Projection, output.pos ).xyww;
	output.tex = input.pos.xyz;

	return output;
}
