#define M_PI 3.14159265359
#define M_RECIPROCAL_PI 0.3183098861837907
#define M_RECIPROCAL_2PI 0.15915494309189535
struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VPS_INOUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : TEXCOORD1;
    row_major float3x3 tbn : TEXCOORD2;
};

struct CameraData
{
    row_major float4x4 view;
    row_major float4x4 proj;
    float4 position;
};

struct LightData
{
    float4 color;
    float4 direction;
};

struct TerrainData
{
    float4 size;
    float heightMapSize;
};

struct WaterData
{
    float4 size;
    float heightMapSize;
};

struct FogData
{
    float4 color;
    float start;
    float end;
    float enable;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    float time;
    CameraData camera;
    LightData light;
    TerrainData terrain;
    WaterData water;
    FogData fog;
};

float3 computePhongLighting(
    CameraData camera,
    LightData light,
    float3 worldPositon,
    float3 normal,
    
    float ka,
    float3 ia,
    
    float kd,
    float3 id,

    float ks,
    float3 is,
    
    float shininess
)
{
    float3 light_direction = -normalize(light.direction.xyz);
    float3 directionToCamera = normalize(worldPositon - camera.position.xyz);

	//AMBIENT LIGHT
    float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
    float amount_diffuse_light = max(dot(light_direction.xyz, normal), 0.0);
    float3 diffuse_light = kd * (light.color.rgb * id) * amount_diffuse_light;

	//SPECULAR LIGHT
    float3 reflected_light = reflect(light_direction.xyz, normal);
    float amount_specular_light = pow(max(0.0, dot(reflected_light, directionToCamera)), shininess);

    float3 specular_light = ks * amount_specular_light * is;

    float3 final_light = ambient_light + diffuse_light + specular_light;

    return final_light;
}

float3 computeNormalFromHeightMa(
    Texture2D heightMap,
    sampler heightMapSampler,
    float heightMapSize,
    float2 texcoord,
    float normalFactor
)
{
    float texelSize = 1.0 / heightMapSize;
    
    float t = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x, texcoord.y - texelSize), 0).r;
    float b = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x, texcoord.y + texelSize), 0).r;
    float l = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x - texelSize, texcoord.y), 0).r;
    float r = heightMap.SampleLevel(heightMapSampler, float2(texcoord.x + texelSize, texcoord.y), 0).r;

    t *= normalFactor;
    b *= normalFactor;
    l *= normalFactor;
    r *= normalFactor;

    float3 normal = float3(-(r - l) * 0.5, 1, -(b - t) * 0.5);
    
    return normalize(normal);

}

float computeFog(
    float distance,
    float start,
    float end
)
{
    float res = (end - distance) / (end - start);
    return clamp(res, 0, 1);
}

float3 computeFogColor(
    FogData fog,
    CameraData camera,
    float3 worldPosition,
    float3 sceneColor
)
{
    float3 dir = worldPosition - camera.position.xyz;
    float dist = length(dir);
    float fogAmount = computeFog(dist, fog.start, fog.end);
    
    if(fog.enable == false)
        return sceneColor.rgb;
    
    return lerp(fog.color.rgb, sceneColor.rgb, fogAmount);
}

float nrand(float2 uv, float time)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233)))* time * 43758.5453);
}