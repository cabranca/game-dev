#type vertex
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(model * vec4(pos, 1.0));

    // This should be done in CPU as the inverse is expensive
    Normal = mat3(transpose(inverse(model))) * normal; 

    TexCoords = texCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
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

struct LightComponents {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
  
    LightComponents components;
};

struct PointLight {    
    vec3 position;
    
    LightComponents components;

    float constant;
    float linear;
    float quadratic;  
};

// Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;

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
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
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
    vec3 ambient  = light.components.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.components.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.components.specular * spec * vec3(texture(material.specular, TexCoords));
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
    vec3 ambient  = light.components.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.components.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.components.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
