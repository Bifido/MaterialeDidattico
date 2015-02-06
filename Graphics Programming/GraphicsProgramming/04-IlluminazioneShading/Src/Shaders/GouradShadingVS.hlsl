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

cbuffer Material : register(cb1)
{
	float4 Ambiental;
	float4 Diffusive;
	float4 Specular;
	float4 Emissive;
	float Shininess;
};

#define MaxLights 5
cbuffer LightsBuffer : register(cb2)
{
	float4 LightPosition[MaxLights];
	float4 LightColor[MaxLights];
	float4 LightAmbient[MaxLights];
	float4 LightAttenuation[MaxLights];
	int UsedLights;
};



struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
	float4 col : COLOR0;
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

PixelShaderInput GouradShadingVS(VertexShaderInput input)
{
	PixelShaderInput output;

	float4 worldSpacePos = mul(World, float4(input.pos, 1.0f));
	float4 cameraSpacePos = mul(View, worldSpacePos);
	float3 worldSpaceNormal = mul((float3x3)World, input.norm);
	float3 viewDir = normalize(CameraPosition - worldSpacePos.xyz);

	output.pos = mul(Projection, cameraSpacePos);
	output.col = CalcLightinig(worldSpacePos.xyz, worldSpaceNormal, viewDir);
	return output;
}
