//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

TextureCube envTex : register( t0 );
SamplerState samLinear  : register( s0 );

#define MaxLights 5
cbuffer LightsBuffer : register(cb0)
{
	float4 LightPosition[MaxLights];
	float4 LightColor[MaxLights];
	float4 LightAmbient[MaxLights];
	float4 LightAttenuation[MaxLights];
	int UsedLights;
};

struct PixelShaderInput
{
    float4 pos				: SV_POSITION;
	float3 viewDirection	: TEXCOORD0;
	float3 wNormal			: NORMAL;
	float3 wPosition		: TEXCOORD1;
};

float4 EnvironmentMapPS(PixelShaderInput input) : SV_TARGET
{
	float3 normal = normalize(input.wNormal);
	float3 view = normalize(input.viewDirection);
	float3 reflect = 2 * normal * dot(normal, view) - view;

	float4 specular = float4(envTex.Sample(samLinear, normalize(reflect)).rgb, 1);

	float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for(int i = 0; i < UsedLights; ++i)
	{
		float3 lightDir = normalize(LightPosition[i].xyz - input.wPosition);
		outColor += saturate(dot( lightDir, normal ) * LightColor[i] * specular);
		outColor += LightAmbient[i] * specular;
	}

	return float4(outColor.rgb, 1.0f);
}
