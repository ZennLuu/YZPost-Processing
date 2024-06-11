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

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
    float3 color = Color.Sample(ColorSampler, uv).rgb;
    //float3 color, hdrcolor;
    //const float gamma = 1.0 / 2.2;
    //float exposure = 1.0;
    
    //hdrcolor = Color.Sample(ColorSampler, uv).rgb;
    
    //color = float3(1, 1, 1) - exp(-hdrcolor * exposure);
    
    //color = pow(color, float3(gamma, gamma, gamma));
    
    return float4(color, 1.0);
}
