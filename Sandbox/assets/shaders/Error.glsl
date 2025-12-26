#type vertex
#version 460 core

layout (location = 0) in vec3 pos;

layout(std140, binding = 0) uniform SceneData {
    mat4 u_ViewProjection;
};

uniform mat4 u_Model;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(pos, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0, 0.0, 1.0, 1.0);
}