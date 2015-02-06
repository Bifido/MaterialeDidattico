//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

cbuffer Material : register(cb0)
{
	float4 Ambiental;
	float4 Diffusive;
	float4 Specular;
	float4 Emissive;
	float Shininess;
};

#define MaxLights 5
cbuffer LightsBuffer : register(cb1)
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
	float3 wNormal			: NORMAL;
	float3 viewDirection	: VIEWDIRECTION;
	float3 wPos				: WORLDPOSITION;
};

struct PixelShaderOutput
{
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
};

float4 CalcLightinig(float3 worldPosition, float3 worldSpaceNormal, float3 viewDirection)
{
	float3 color = float3(0.0f, 0.0f, 0.0f);
	color += Emissive.rgb;

	for(int i = 0; i < UsedLights; ++i)
	{
		float3 worldToLight = LightPosition[i].xyz - worldPosition;
		float3 lightDir = normalize(worldToLight);
		float lightDist = length( worldToLight );
        float fAtten = 1.0f / dot( LightAttenuation[i].xyz, float3(1.0f, lightDist, lightDist*lightDist) );

		color += Ambiental.rgb * LightAmbient[i].rgb;

		float kDiffusive = saturate(dot(lightDir, worldSpaceNormal));
		color += kDiffusive * Diffusive.rgb * LightColor[i].rgb * fAtten;

		float3 halfVector = normalize( (lightDir + viewDirection));
		float kSpecular = saturate(dot(halfVector, worldSpaceNormal));
		kSpecular = pow(kSpecular, Shininess);
		color += kSpecular * Specular.rgb * LightColor[i].rgb * fAtten;
	}
	return float4(color, 1.0f);
}

PixelShaderOutput PhongShadingPS(PixelShaderInput input)
{
	PixelShaderOutput output;
	float3 norm = normalize(input.wNormal);
	output.color = CalcLightinig(input.wPos, norm, normalize(input.viewDirection));

	float3 biasedNormal = norm + float3(1.0f, 1.0f, 1.0f);
	biasedNormal *= float3(0.5f, 0.5f, 0.5f);
	output.normal = float4(biasedNormal, 1.0f);
    return output;
}