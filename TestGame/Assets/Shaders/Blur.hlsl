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

Texture2D Depth : register(t1);
SamplerState DepthSampler : register(s1);

float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
// Hill ACES HDR
float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
    float3 color = Color.Sample(ColorSampler, uv).rgb;
    float brightness = dot(color, float3(0.2126, 0.7152, 0.0722));
    
    float4 color4;
    if (brightness > 1.0)
        color4 = float4(color, 1.0);
    else
        color4 = float4(0, 0, 0, 1.0);
    return color4;
}