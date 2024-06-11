#include "Common.hlsl"

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;

	//WORLD SPACE
    output.position = mul(input.position, world);
    output.worldPosition = output.position.xyz;

    output.position = mul(output.position, camera.view);
    output.position = mul(output.position, camera.proj);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float3x3) world));
    return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

Texture2D Normal : register(t1);
sampler NormalSampler : register(s1);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
    float4 color = Color.Sample(ColorSampler, uv);
    
    float3 final_light = computePhongLighting(
        camera,
        light,
        input.worldPosition.xyz,
        input.normal.xyz,
        
        4,
        color.rgb * float3(0.09,0.09,0.09),
        
        0.7,
        color.rgb,
    
        0.0,
        float3(1, 1, 1),
    
        10.0);

    final_light = computeFogColor(fog, camera, input.worldPosition.xyz, final_light);
    
    color.rgb = final_light;
    
    return float4(final_light, 1.0);
}