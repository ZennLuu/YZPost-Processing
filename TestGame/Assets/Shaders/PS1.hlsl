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

float dithering_pattern(float2 fragcoord)
{
    const float pattern[] =
    {
        0.00, 0.50, 0.10, 0.65,
		0.75, 0.25, 0.90, 0.35,
		0.20, 0.70, 0.05, 0.50,
		0.95, 0.40, 0.80, 0.30
    };
	
    int x = fragcoord.x % 4;
    int y = fragcoord.y % 4;
	
    return pattern[y * 4 + x];
}

#define MAXCOLORS 16

float reduce_color(float raw, float dither, int depth)
{
    float div = 1.0 / float(depth);
    float val = 0.0;
    int i = 0;

    while (i <= MAXCOLORS)
    {
        if (raw > div * (float(i + 1)))
        {
            i = i + 1;
            continue;
        }

        if (raw * float(depth) - float(i) <= dither * 0.999)
        {
            val = div * float(i);
        }
        else
        {
            val = div * float(i + 1);
        }
        return val;

        i = i + 1;
    }

    return val;
}

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
 
    float4 color = Color.Sample(ColorSampler, uv);

    int dither_size = 1;
    
    float2 uv2 = uv / dither_size;
    
    float dithering_value = 1.0;
    dithering_value = dithering_pattern(uv);
    
    color.r = reduce_color(color.r, (dithering_value - 0.5) * dithering_value + 0.5, 12 - 1);
    color.g = reduce_color(color.g, (dithering_value - 0.5) * dithering_value + 0.5, 12 - 1);
    color.b = reduce_color(color.b, (dithering_value - 0.5) * dithering_value + 0.5, 12 - 1);
    
    return float4(color);
}