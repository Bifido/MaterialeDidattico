//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

float4 SimpleVertexShader(float3 pos : POSITION) : SV_POSITION{
    return float4(pos, 1.0f);
}