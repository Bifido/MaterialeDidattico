//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

Texture2D tex : register( t0 );
SamplerState texSampler : register( s0 );

struct PixelShaderInput
{
    float4 pos	: SV_POSITION;
	float2 coord : TEXCOORD;
};

float4 FullScreenQuadPS(PixelShaderInput input) : SV_TARGET
{
	float3 color = tex.Sample(texSampler, input.coord).rgb;
	return float4(color, 1.0f);
}
