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
	float3 norm : NORMAL;
};

struct GeometryShaderInput
{
	float4 pos	: SV_POSITION;
	float3 norm : NORMAL;
};

GeometryShaderInput PhongShadingVS(VertexShaderInput input)
{
	GeometryShaderInput output;

	output.pos = mul(World, float4(input.pos, 1.0f));
	output.norm = mul((float3x3)World, input.norm);

	return output;
}
