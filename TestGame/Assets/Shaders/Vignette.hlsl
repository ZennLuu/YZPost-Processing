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

float vignette(float2 uv)
{
    uv *= 1.0 - uv.xy;
    float vignette = uv.x * uv.y * 15.0;
    float vignette_intensity = 0.4;
    float vignette_opacity = 0.5;
    return pow(abs(vignette), vignette_intensity * vignette_opacity);
}

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
 
    float4 color = Color.Sample(ColorSampler, uv);
    
    // set vignette color
    float4 vig = float4(0, 0, 0, 1);
    
    float4 color1 = color;
    
    color1 *= vig;
    color1 *= (1.0 - vignette(uv));
    
    return float4(color.rgb /*+ color1.rgb*/, 1.0 - color1.a);
}