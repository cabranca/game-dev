#type vertex
#version 460 core

struct DirLight {
    vec3 direction;
    vec3 radiance;
};

layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    DirLight u_DirLight;
    vec3 u_CameraPosition; // Agregamos la cámara acá
};

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 u_Model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(u_Model * vec4(pos, 1.0));

    // This should be done in CPU as the inverse is expensive
    Normal = mat3(transpose(inverse(u_Model))) * normal; 

    TexCoords = texCoords;
    
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
} 

#type fragment
#version 460 core
// Inputs and Outputs
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoords;

// Structs
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct DirLight {
    vec3 direction;
    vec3 radiance;
};

struct PointLight {    
    vec3 position;
    vec3 radiance;

    float constant;
    float linear;
    float quadratic;  
};

// --- UBO (Datos Globales) ---
// Reemplaza a: uniform DirLight dirLight; y uniform vec3 viewPos;
layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
    DirLight u_DirLight;    // Ahora viene del buffer
    vec3 u_CameraPosition;  // Ahora viene del buffer
};

// Uniforms
uniform Material material;

// SSBO
layout(std430, binding = 0) readonly buffer LightBuffer {
    int u_PointLightCount;
    int _pad[3];
    PointLight pointLights[];
} u_Lights;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_CameraPosition - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < u_Lights.u_PointLightCount; i++)
        result += CalcPointLight(u_Lights.pointLights[i], norm, FragPos, viewDir);    
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.radiance  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.radiance  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.radiance * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.radiance * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.radiance * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.radiance * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
