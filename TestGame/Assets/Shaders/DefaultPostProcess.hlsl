#include "Common.hlsl"

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    
    output.position = input.position;
    output.texcoord = input.texcoord;
    return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    return Color.Sample(ColorSampler, input.texcoord);
}
