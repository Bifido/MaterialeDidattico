//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

struct VertexShaderInput
{
	float3 pos : POSITION;
	float4 col : COLOR;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
	float4 col : COLOR0;
};

PixelShaderInput SimpleVertexShader(VertexShaderInput input)
{
    PixelShaderInput vertexShaderOutput;

    vertexShaderOutput.pos = float4(input.pos, 1.0f);
	vertexShaderOutput.col = input.col;
    return vertexShaderOutput;
}