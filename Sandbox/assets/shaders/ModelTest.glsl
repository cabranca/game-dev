#type vertex
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = texCoords;    
    gl_Position = projection * view * model * vec4(pos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{    
    vec4 diffuse = texture(texture_diffuse1, TexCoords);
    vec4 specular = texture(texture_specular1, TexCoords);
    vec4 normal = texture(texture_normal1, TexCoords);
    
    FragColor = diffuse + specular * 0.0001 + normal * 0.0001; 
}