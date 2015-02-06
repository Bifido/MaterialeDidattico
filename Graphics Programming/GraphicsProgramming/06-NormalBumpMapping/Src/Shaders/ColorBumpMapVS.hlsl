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
	float3 pos		: POSITION;
	float3 norm		: NORMAL;
	float3 tang		: TANGENT;
	float3 biNorm	: BINORMAL;
	float2 texCoord	: TEXCOORD;
};

struct PixelShaderInput
{
    float4 pos				: SV_POSITION;
	float2 texCoord			: TEXCOORD0;
	float3 wNormal			: NORMAL;
	float3 wTangent			: TANGENT;
	float3 wBinormal		: BINORMAL;
	float3 viewDirection	: TEXCOORD2;
	float3 wPos				: TEXCOORD3;
};

PixelShaderInput ColorBumpMapVS(VertexShaderInput input)
{
	PixelShaderInput output;

	float4 worldSpacePos = mul(World, float4(input.pos, 1.0f));
	float4 cameraSpacePos = mul(View, worldSpacePos);

	output.pos = mul(Projection, cameraSpacePos);
	output.texCoord = input.texCoord;
	output.wNormal = normalize(mul((float3x3)World, input.norm));
	output.wTangent = normalize(mul((float3x3)World, input.tang));
	output.wBinormal = normalize(mul((float3x3)World, input.biNorm));
	output.viewDirection = normalize(CameraPosition - worldSpacePos.xyz);
	output.wPos = worldSpacePos.xyz;

	return output;
}