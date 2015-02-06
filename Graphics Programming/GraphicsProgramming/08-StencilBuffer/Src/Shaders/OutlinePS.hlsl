//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

cbuffer OulineParams : register(cb0)
{
	float4 DefaultColor;
}

float4 OutlinePS( float4 pos: SV_POSITION ) : SV_TARGET
{
	return DefaultColor;
}
