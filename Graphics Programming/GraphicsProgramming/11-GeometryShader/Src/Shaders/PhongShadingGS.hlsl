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

cbuffer GeometryBuffer : register(cb1)
{
	float ExplodeFactor;
};

struct GeometryShaderInput
{
	float4 pos	: SV_POSITION;
	float3 norm : NORMAL;
};

struct PixelShaderInput
{
    float4 pos				: SV_POSITION;
	float3 wNormal			: NORMAL;
	float3 viewDirection	: VIEWDIRECTION;
	float3 wPos				: WORLDPOSITION;
};

[maxvertexcount(3)]
void PhongShadingGS(triangle GeometryShaderInput input[3], inout TriangleStream<PixelShaderInput> TriStream)
{
	float3 faceEdgeA = input[1].pos.xyz - input[0].pos.xyz;
	float3 faceEdgeB = input[2].pos.xyz - input[0].pos.xyz;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	float3 explodeAmt = faceNormal * ExplodeFactor;

	for(int i = 0; i < 3; ++i)
	{
		PixelShaderInput output;

		output.wPos = input[i].pos.xyz + explodeAmt;
		output.pos = mul(View, float4(output.wPos, 1.0f));
		output.pos = mul(Projection, output.pos);
		output.wNormal = input[i].norm;
		output.viewDirection = normalize(CameraPosition - output.wPos);
		TriStream.Append(output);
	}

	TriStream.RestartStrip();
}

