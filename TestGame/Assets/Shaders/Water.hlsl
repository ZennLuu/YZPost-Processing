#include "Common.hlsl"

Texture2D HeightMap : register(t0);
sampler HeightMapSampler : register(s0);



VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    
    float2 texcoord = (input.texcoord * 8) + time * 0.009;
    float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 8) + time * 0.01;
    
    float height1 = HeightMap.SampleLevel(HeightMapSampler, texcoord, 0).r;
    float height2 = HeightMap.SampleLevel(HeightMapSampler, texcoord2, 0).r;
    
    float height = lerp(height1, height2, 0.5);
    
    output.position = mul(float4(
		input.position.x * water.size.x,
		height * water.size.y,
		input.position.z * water.size.z,
		1),
		world);
    output.worldPosition = output.position.xyz;

    output.position = mul(output.position, camera.view);
    output.position = mul(output.position, camera.proj);
    output.texcoord = input.texcoord;

    return output;
}



float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 texcoord = (input.texcoord * 8) + time * 0.009;
    float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 8) + time * 0.01;
    
    float3 normal1 = computeNormalFromHeightMa(
        HeightMap,
        HeightMapSampler,
        water.heightMapSize,
        texcoord,
        water.size.y * 3);
    
    float3 normal2 = computeNormalFromHeightMa(
        HeightMap,
        HeightMapSampler,
        water.heightMapSize,
        texcoord2,
        water.size.y * 3);
    
    float3 normal = normalize(lerp(normal1, normal2, 0.5));
    normal = normalize(mul(normal, (float3x3) world));
    
    float4 waterColor = float4(0.24, 0.34, 0.49, 1); //GroundMap.Sample(GroundMapSampler, input.texcoord * 100.0);
    float4 crestColor = float4(1, 1, 1, 1); //CliffMap.Sample(CliffMapSampler, input.texcoord * 60.0);

    float4 color = waterColor;
    float angle = abs(dot(normal, float3(0, 1, 0)));

    float minAngle = 0.3;
    float maxAngle = 1.0;

    if (angle >= minAngle && angle <= maxAngle)
        color = lerp(crestColor, color, (angle - minAngle) * (1.0 / (maxAngle - minAngle)));
    if (angle < minAngle)
        color = crestColor;

    float3 final_light = computePhongLighting(
        camera,
        light,
        input.worldPosition.xyz,
        normal.xyz,
        
        4,
        color.rgb * float3(0.09, 0.09, 0.09),
        
        0.7,
        color.rgb,
    
        0.2,
        light.color.rgb,
    
        100.0);
    
    final_light = computeFogColor(fog, camera, input.worldPosition.xyz, final_light);
    
    return float4(final_light, 0.8);
}