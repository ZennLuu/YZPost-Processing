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
    float4 position;
    int type;
    float intensity;
    //float radius;
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

struct MaterialData
{
    float metallic ;
    float roughness;
    float reflectance;
};

struct MulLightData
{
    int lightnumber;
    int lightindex;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    float time;
    CameraData camera;
    TerrainData terrain;
    WaterData water;
    FogData fog;
    MaterialData material;
    LightData light[16];
    MulLightData lights;
};

float3 computePhongLighting(
    CameraData camera,
    LightData light[16],
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
    float3 final_light = 0;
    
    for (int i = 0; i < 16; i++)
    {
        if (i >= lights.lightnumber)
            break;
            
        float3 light_direction = normalize(-light[i].direction.xyz);
        float3 directionToCamera = normalize(worldPositon - camera.position.xyz);
    
        light[i].color.rgb *= light[i].intensity;
        
        float attenuation = 1.0;
        if (light[i].intensity == 0.0)
        {
            attenuation = 0.0;
        }
        else
        {
            if (light[i].type == 1)
            {
            // Point light
                light_direction = normalize(light[i].position.xyz - worldPositon.xyz);
        
                float distance = length(light[i].position.xyz - worldPositon.xyz);
                attenuation = 1.0 / (0.032 * distance * distance + 0.09 * distance + 1.0);
            }
            else if (light[i].type == 2)
            {
                // Spot light
                light_direction = normalize(light[i].position.xyz - worldPositon.xyz);
        
                float outerCone = 0.90;
                float innerCone = 0.95;
        
                float angle = dot(normalize(light[i].direction.xyz), -light_direction);
                attenuation = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);
            }
        }
    
	    //AMBIENT LIGHT
        float3 ambient_light = ka * ia * attenuation;

	    //DIFFUSE LIGHT
        float amount_diffuse_light = max(dot(light_direction.xyz, normal), 0.0);
    
        float3 diffuse_light = kd * (light[i].color.rgb * id) * amount_diffuse_light * attenuation;
	
        //SPECULAR LIGHT
        float3 reflected_light = reflect(light_direction.xyz, normal);
        float amount_specular_light = pow(max(0.0, dot(reflected_light, directionToCamera)), shininess);

        float3 specular_light = ks * amount_specular_light * is * attenuation;

        final_light += ambient_light + diffuse_light + specular_light;
    }

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