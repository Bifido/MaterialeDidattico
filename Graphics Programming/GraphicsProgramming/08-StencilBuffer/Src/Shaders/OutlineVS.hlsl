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
	float3 pos	: POSITION;
	float3 norm : NORMAL;
};

float4 OutlineVS( VertexShaderInput input ): SV_POSITION
{   
    float4 output = mul( World, float4(input.pos, 1.0f) );
    output = mul( View, output );
    output = mul( Projection, output );
    	
    return output;
}
