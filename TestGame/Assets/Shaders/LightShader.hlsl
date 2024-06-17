#include "Common.hlsl"

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;

	//WORLD SPACE
    output.position = mul(input.position, world);
    output.position = mul(output.position, camera.view);
    output.position = mul(output.position, camera.proj);
    output.texcoord = input.texcoord;

    return output;
}

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    return float4(light.color.rgb * light.intensity, 1.0);
}
