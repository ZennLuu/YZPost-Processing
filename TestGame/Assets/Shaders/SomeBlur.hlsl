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
    float4 bloom = 0;
    float span = 10;
    //float span = 6 + 5 * sin(5 * time);
    int tt = abs(span);
    for (int i = -tt; i < tt; ++i)
    {
        float ofs = i;
        //bloom += Color.Sample(ColorSampler, input.texcoord.xy + float2(ofs / 100, 0)) / (2 * tt + 1);
        bloom += Color.Sample(ColorSampler, input.texcoord.xy + float2(0, ofs / 100)) / (2 * tt + 1);

    }
        float2 uv = input.texcoord.xy;
    float3 hdrcolor = Color.Sample(ColorSampler, uv).rgb;

    
    return Color.Sample(ColorSampler, input.texcoord.xy) + bloom;

}