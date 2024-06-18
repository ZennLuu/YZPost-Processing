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
    float4 color;
    float2 uv = input.texcoord.xy;
    
    float2 pos = float2((uv.x - 0.5) * 2.0, (uv.y - 0.5) * 2);
    float l = length(pos);
    
    float red_shift = 0.009;
    float green_shift = 0.004;
    float blue_shift = -0.007;
    
    //color.r = Color.Sample(ColorSampler, uv + ((sin(time) + red_shift) * red_shift)).r;
    //color.g = Color.Sample(ColorSampler, uv + ((sin(time) + green_shift) * green_shift)).g;
    //color.b = Color.Sample(ColorSampler, uv + ((sin(time) + blue_shift) * blue_shift)).b;
    //color.a = 1.0;
    
    color.r = Color.Sample(ColorSampler, uv + l * red_shift).r;
    color.g = Color.Sample(ColorSampler, uv + l * green_shift).g;
    color.b = Color.Sample(ColorSampler, uv + l * blue_shift).b;
    color.a = 1.0;
    
    
    return color;

}