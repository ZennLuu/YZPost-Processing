#include "Common.hlsl"

VPS_INOUT vsmain(VPS_INOUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;

	//WORLD SPACE
    output.position = mul(input.position, world);
    output.position = mul(output.position, camera.view);
    output.position = mul(output.position, camera.proj);
    output.texcoord = input.texcoord;

    return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    return Color.Sample(ColorSampler, input.texcoord);

}