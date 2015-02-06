//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

Texture2D colorTex : register( t0 );
Texture2D normalTex : register( t1 );
SamplerState texSampler : register( s0 );

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
	float2 texCoord			: TEXCOORD0;
	float3 wNormal			: NORMAL;
	float3 wTangent			: TANGENT;
	float3 wBinormal		: BINORMAL;
	float3 viewDirection	: TEXCOORD2;
	float3 wPos				: TEXCOORD3;
};

float4 CalcLightinig(float3 worldPosition, float3 worldSpaceNormal, float3 viewDirection, float3 materialColor)
{
	float3 color = float3(0.0f, 0.0f, 0.0f);
	color += Emissive.rgb * materialColor;

	for(int i = 0; i < UsedLights; ++i)
	{
		float3 worldToLight = LightPosition[i].xyz - worldPosition;
		float3 lightDir = normalize(worldToLight);
		float lightDist = length( worldToLight );
        float fAtten = 1.0f / dot( LightAttenuation[i].xyz, float3(1.0f, lightDist, lightDist*lightDist) );

		color += Ambiental.rgb * LightAmbient[i].rgb * materialColor;

		float kDiffusive = saturate(dot(lightDir, worldSpaceNormal));
		color += kDiffusive * Diffusive.rgb * LightColor[i].rgb * fAtten * materialColor;

		float3 halfVector = normalize( (lightDir + viewDirection));
		float kSpecular = saturate(dot(halfVector, worldSpaceNormal));
		kSpecular = pow(kSpecular, Shininess);
		color += kSpecular * Specular.rgb * LightColor[i].rgb * fAtten * materialColor;
	}

	return float4(color, 1.0f);
}

float4 ColorBumpMapPS(PixelShaderInput input) : SV_TARGET
{
	float3 materialColor = colorTex.Sample(texSampler, input.texCoord).rgb;
	float3 bumpMap = normalTex.Sample(texSampler, input.texCoord).xyz;

	float3x3 worldToTangent = { input.wTangent, input.wBinormal, input.wNormal };

	float3 bumpNormal = mul(transpose(worldToTangent), bumpMap);

    return CalcLightinig(input.wPos, normalize(bumpNormal), normalize(input.viewDirection), materialColor);
}
