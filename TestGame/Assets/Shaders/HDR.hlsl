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

float luminance(float3 color)
{
    return dot(color, float3(0.299f, 0.587f, 0.114f));
}
float4 ToneMap(float4 hdrColor)
{
    float luminance = dot(hdrColor.rgb, float3(0.2126, 0.7152, 0.0722));
    float mappedLuminance = luminance / (luminance + 1.0);
    float4 mappedColor = hdrColor * (mappedLuminance / luminance);
    
    return mappedColor;
}

static const float3x3 ACESInputMat =
{
    { 0.59719, 0.35458, 0.04823 },
    { 0.07600, 0.90834, 0.01566 },
    { 0.02840, 0.13383, 0.83777 }
};

static const float3x3 ACESOutputMat =
{
    { 1.60475, -0.53108, -0.07367 },
    { -0.10208, 1.10813, -0.00605 },
    { -0.00327, -0.07276, 1.07602 }
};

float3 RRTAndODTFit(float3 v)
{
    float3 a = v * (v + 0.0245786f) - 0.000090537f;
    float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
// Hill ACES HDR
float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
    float3 hdrcolor = Color.Sample(ColorSampler, uv).rgb;
    float3 color = Color.Sample(ColorSampler, uv).rgb;
   
    hdrcolor = mul(ACESInputMat, hdrcolor);
    
    hdrcolor = RRTAndODTFit(hdrcolor);
    
    float3 Cout = mul(ACESOutputMat, hdrcolor);
    
    return float4(saturate(Cout), 1.0);
    
    float brightness = dot(color, float3(0.2126, 0.7152, 0.0722));
    
    float4 color4;
    if (brightness > 1.0)
        return color4 = float4(color, 1.0);
    else
        return color4 = float4(0, 0, 0, 1.0);
    
    
    float2 texOffset;
    Color.GetDimensions(texOffset.x, texOffset.y);
    texOffset = 1.0 / texOffset;
    float3 res = color4.rgb * weight[0];
    
}