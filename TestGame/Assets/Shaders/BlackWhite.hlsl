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
 
    float4 color = Color.Sample(ColorSampler, uv);
    
    float avg = (color.r + color.g + color.b) / 3.0;
    
    float bw;
    
    if (avg < 0.4)
    {
        bw = 0.0;
    }
    else
    {
        bw = 1.0;
    }
    
    return (/*color **/ float4(bw, bw, bw, color.a));
}