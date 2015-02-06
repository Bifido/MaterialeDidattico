//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

struct VertexShaderInput
{
	float3 pos	: POSITION;
	float2 coord : TEXCOORD;
};

struct PixelShaderInput
{
    float4 pos	: SV_POSITION;
	float2 coord : TEXCOORD;
};

PixelShaderInput FullScreenQuadVS(VertexShaderInput input)
{
	PixelShaderInput output;
	output.pos = float4(input.pos, 1.0f);
	output.coord = input.coord;
	return output;
}
