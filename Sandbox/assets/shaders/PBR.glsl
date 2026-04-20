#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;

// --- UBO: Datos de Escena (Fase 1) ---
struct DirLight {
    vec3 direction;
    float _pad0;
    vec3 radiance;
    float _pad1;
};

layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    DirLight u_DirLight;
    vec3 u_CameraPosition;
};

uniform mat4 u_Model;

// Outputs al Fragment Shader
out vec3 v_WorldPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out mat3 v_TBN;

void main()
{
    v_TexCoord = a_TexCoord;
    
    // Transformar posición al mundo
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;

    // Transformar normales (inversa transpuesta para corregir escalas no uniformes)
    mat3 normalMatrix = mat3(transpose(inverse(u_Model)));
    v_Normal = normalMatrix * a_Normal;

    // Construir TBN matrix desde tangent attribute
    vec3 T = normalize(normalMatrix * a_Tangent);
    vec3 N = normalize(v_Normal);
    // Re-ortogonalizar T respecto a N (Gram-Schmidt)
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    v_TBN = mat3(T, B, N);

    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 460 core

layout(location = 0) out vec4 o_Color;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in mat3 v_TBN;

// --- ESTRUCTURAS DE LUCES ---
struct DirLight {
    vec3 direction;
    float _pad0;
    vec3 radiance;
    float _pad1;
};

struct PointLight {    
    vec3 position;
    vec3 radiance;
    float constant;
    float linear;
    float quadratic;  
};

// --- BINDINGS DE DATOS (Coinciden con C++) ---

// UBO: Escena
layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    DirLight u_DirLight;
    vec3 u_CameraPosition;
};

// SSBO: Luces Puntuales (Fase 1 Update)
layout(std430, binding = 0) readonly buffer LightBuffer {
    int u_PointLightCount;
    int _pad[3];
    PointLight pointLights[];
} u_Lights;

// --- UNIFORMS DE MATERIAL (Fase 3 - PBRMaterial) ---
uniform vec3 u_AlbedoColor;
uniform float u_Metalness;
uniform float u_Roughness;

uniform sampler2D u_AlbedoMap;      // Slot 0
uniform sampler2D u_NormalMap;      // Slot 1
uniform sampler2D u_MetalRoughMap;  // Slot 2 (B=Metal, G=Roughness usually)
uniform sampler2D u_AOMap;          // Slot 3

// --- CONSTANTES PBR ---
const float PI = 3.14159265359;

// --- FUNCIONES MATH PBR (Cook-Torrance) ---

// 1. Distribución Normal (Trowbridge-Reitz GGX)
// Calcula qué proporción de micro-facetas están alineadas con el vector H (Halfway)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 0.0000001);
}

// 2. Geometría (Smith's Schlick-GGX)
// Calcula cuánta luz es bloqueada por las propias micro-facetas (autoculling)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// 3. Fresnel (Fresnel-Schlick)
// Calcula cuánto refleja la superficie según el ángulo de visión
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// --- UTILIDADES ---

// Calcula la normal perturbada usando la TBN matrix del vertex shader
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).xyz * 2.0 - 1.0;
    return normalize(v_TBN * tangentNormal);
}

// --- MAIN ---
void main()
{
    // 1. Obtener datos del Material
    vec3 albedo = pow(texture(u_AlbedoMap, v_TexCoord).rgb, vec3(2.2)) * u_AlbedoColor; // Gamma -> Linear
    
    // Asumimos empaquetado standard GLTF: B = Metal, G = Roughness. Si usas texturas separadas, ajustá esto.
    // Si usás texturas "White" default, esto se multiplica por los uniform floats.
    vec4 mrSample = texture(u_MetalRoughMap, v_TexCoord);
    float metallic = mrSample.b * u_Metalness;
    float roughness = mrSample.g * u_Roughness;
    float ao = texture(u_AOMap, v_TexCoord).r;

    // 2. Configurar Vectores Principales
    vec3 N = getNormalFromMap();
    vec3 V = normalize(u_CameraPosition - v_WorldPos);

    // F0: Reflectancia base (0.04 para dieléctricos/plástico, Color de Albedo para metales)
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0); // Luz saliente acumulada

    // --- LOOP DE LUCES ---
    
    // A. Luz Direccional (UBO)
    {
        vec3 L = normalize(-u_DirLight.direction);
        vec3 H = normalize(V + L);
        vec3 radiance = u_DirLight.radiance;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }

    // B. Luces Puntuales (SSBO)
    for(int i = 0; i < u_Lights.u_PointLightCount; ++i) 
    {
        PointLight light = u_Lights.pointLights[i];
        vec3 L = normalize(light.position - v_WorldPos);
        vec3 H = normalize(V + L);
        
        float distance = length(light.position - v_WorldPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        vec3 radiance = light.radiance * attenuation;

        // Cook-Torrance BRDF (Misma lógica)
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 evita división por cero
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }

    // --- AMBIENTE & POST-PROCESADO ---
    
    // Ambiente simple (temporal hasta tener IBL - Image Based Lighting)
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    // Tone Mapping (Reinhard) - PBR produce colores > 1.0 (HDR), hay que bajarlos a monitor (LDR)
    color = color / (color + vec3(1.0));

    // Gamma Correction (Linear -> sRGB)
    color = pow(color, vec3(1.0/2.2)); 

    o_Color = vec4(color, 1.0);
}
