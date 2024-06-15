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
    output.tbn[0] = normalize(mul(input.tangent, (float3x3) world));
    output.tbn[1] = normalize(mul(input.binormal, (float3x3) world));
    output.tbn[2] = output.normal;
    return output;
}

float3 rgb2lin(float3 rgb)
{ // sRGB to linear approximation
    return pow(rgb, float3(2.2, 2.2, 2.2));
}

float3 lin2rgb(float3 lin)
{ // linear to sRGB approximation
    return pow(abs(lin), float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
}

float3 fresnelShlick(float cosTheta, float3 f0)
{
    return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}

float D_GGX(float NoH, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NoH2 = NoH * NoH;
    float b = (NoH2 * (alpha2 - 1.0) + 1.0);
    return alpha2 * M_RECIPROCAL_PI / (b * b);
}

float G1_GGX_Shlick(float NoV, float roughness)
{
    float alpha = roughness * roughness;
    float k = alpha / 2.0;
    return max(NoV, 0.001) / (NoV * (1.0 - k) + k);
}

float G_Smith(float NoV, float NoL, float roughness)
{
    return G1_GGX_Shlick(NoL, roughness) * G1_GGX_Shlick(NoV, roughness);
}

float3 microfacetBRDF(float3 L, float3 V, float3 N, 
                        float metallic, float roughness, float3 baseColor, float reflectance)
{
    float3 H = normalize(V + L);
    
    float NoV = clamp(dot(N, V), 0.0, 1.0);
    float NoL = clamp(dot(N, L), 0.0, 1.0);
    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);
    
    float a16 = 0.16 * (roughness * roughness);
    
    float3 f0 = float3(a16, a16, a16);
    f0 = lerp(f0, baseColor, metallic);
    
    float3 F = fresnelShlick(VoH, f0);
    float D = D_GGX(NoH, roughness);
    float G = G_Smith(NoV, NoL, roughness);
 
    float3 spec = (F * D * G) / (4.0 * max(NoV, 0.001) * max(NoL, 0.001));
    
    float3 rhoD = baseColor;
    
    // optionally
    rhoD *= float3(1.0, 1.0, 1.0) - F;
    // rhoD *= disneyDiffuseFactor(NoV, NoL, VoH, roughness);
    
    rhoD *= (1.0 - metallic);
    
    float3 diff = rhoD * M_RECIPROCAL_PI;
   
    return diff + spec;
    
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

Texture2D Normal : register(t1);
sampler NormalSampler : register(s1);

Texture2D Metallic : register(t2);
sampler MetallicSampler : register(s2);

Texture2D Roughness : register(t3);
sampler RoughnessSampler : register(s3);

TextureCube<float4> CubeMap : register(t4);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float2 uv = input.texcoord.xy;
    float4 color = Color.Sample(ColorSampler, uv);
    float4 normal = Normal.Sample(NormalSampler, uv);
    
    normal.xyz = normalize(normal.xyz * 2.0 - 1.0);
    normal.xyz = mul(normal.xyz, input.tbn);
    
    float3 L, N, V;
   
    L = normalize(-light.direction.xyz);
    N = normal.xyz;
    //N = input.normal.xyz;
    V = normalize(camera.position.xyz - input.position.xyz);
    
    float metallic = Metallic.Sample(MetallicSampler, uv).r;
    float roughness = Roughness.Sample(RoughnessSampler, uv).r;
    float reflectance = 0.5;

    float3 finalColor = rgb2lin(float3(0, 0, 0));
    
    float irradiance = max(dot(light.direction.xyz, N), 0.0) * 10;
    
    if (irradiance > 0.0)
    {
        float3 brdf = microfacetBRDF(L, V, N, metallic, roughness, color.rgb, reflectance);
        finalColor = brdf * irradiance * light.color.rgb;
    }
    
    
    float3 refletionVector = reflect(V, normalize(input.normal.xyz));
    
    float3 reflectColor = CubeMap.Sample(ColorSampler, refletionVector).rgb;
    
    return float4(finalColor, 1.0);
}