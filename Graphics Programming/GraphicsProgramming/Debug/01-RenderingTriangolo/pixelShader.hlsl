//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

float4 SimplePixelShader(float4 pos : SV_POSITION) : SV_TARGET
{
    // Draw the entire triangle yellow.
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}
