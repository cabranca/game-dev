#type vertex
#version 460 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = pos;
    vec4 position = projection * view * model * vec4(pos, 1.0);
    gl_Position = position.xyww;
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    FragColor = texture(u_Skybox, TexCoords);
}