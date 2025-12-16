#type vertex
#version 460 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}
