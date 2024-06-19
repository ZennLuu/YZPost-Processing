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
    float offset = 1.0 / 3000.0;
    
    float2 offsets[9] =
    {
        float2(-offset, offset),
        float2(0, offset),
        float2(offset, offset),
        float2(-offset, 0),
        float2(0, 0),
        float2(offset, 0),
        float2(-offset, -offset),
        float2(0, -offset),
        float2(offset, -offset),
    };
    
    float kernel[9] =
    {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    
    /*
    for blur effect
    float kernel[9] =
    {
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    };
    */
    
    float2 uv = input.texcoord;
    
    float3 sample[9];
    for (int i = 0; i < 9; i++)
        sample[i] = Color.Sample(ColorSampler, uv + offsets[i]).rgb;
    
    float3 color = 0;
    for (int j = 0; j < 9; j++)
        color += sample[j] * kernel[j];
    
    //float brightness = length(color);
    
    //if (brightness > 0.7)
    //    color = float3(1,1,1);
    //else
    //    color = float3(0, 0, 0);
    
    
    return float4(color, 1.0);
}
