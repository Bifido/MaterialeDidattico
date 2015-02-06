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

cbuffer PixelsInfo : register(cb0)
{
	float2 texelDistence;
}

// Single pass 5x5 gaussian filter. Very poor perfomance!!!
float3 gaussianFilter(float2 coord)
{
	float2 offsets[25] = {	float2(-2, -2), float2(-1, -2), float2(0, -2), float2(1, -2), float2(2, -2),
							float2(-2, -1), float2(-1, -1), float2(0, -1), float2(1, -1), float2(2, -1),
							float2(-2,  0), float2(-1,  0), float2(0,  0), float2(1,  0), float2(2,  0),
							float2(-2,  1), float2(-1,  1), float2(0,  1), float2(1,  1), float2(2,  1),
							float2(-2,  2), float2(-1,  2), float2(0,  2), float2(1,  2), float2(2,  2) };
	float gaussMask[25] = { 1,  4,  7,  4, 1,
							4, 16, 26, 16, 4,
							7, 26, 41, 26, 7,
							4, 16, 26, 16, 4,
							1,  4,  7,  4, 1};

	float3 color = float3(0, 0, 0);
	for(int i = 0; i < 25; ++i)
	{
		float2 sampleCoord = coord + offsets[i] * texelDistence;
		float3 sampleColor = tex.Sample(texSampler, sampleCoord).rgb;
		sampleColor *= gaussMask[i];
		color += sampleColor;
	}

	color /= 273;
	return color;
}

float4 GaussianBlurPS(PixelShaderInput input) : SV_TARGET
{
	float3 color = gaussianFilter(input.coord);
	return float4(color, 1.0f);
}
