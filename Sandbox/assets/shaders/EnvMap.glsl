#type vertex
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 Position;

void main()
{
    Normal = mat3(transpose(inverse(model))) * normal;
    Position = vec3(model * vec4(pos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
} 

#type fragment
#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main()
{             
    vec3 I = normalize(Position - u_CameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(u_Skybox, R).rgb, 1.0);
}
