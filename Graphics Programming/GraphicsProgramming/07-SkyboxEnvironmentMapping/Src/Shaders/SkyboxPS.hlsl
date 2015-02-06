//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

TextureCube skyboxTex : register( t0 );
SamplerState samLinear  : register( s0 );

struct PixelShaderInput
{
    float4 pos	: SV_POSITION;
	float3 tex	: TEXCOORD0;
};

float4 SkyboxPS(PixelShaderInput input) : SV_TARGET
{
    return  float4(skyboxTex.Sample(samLinear, normalize(input.tex)).rgb, 1);
}
