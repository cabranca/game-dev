#type vertex
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in float texIndex;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex;

void main()
{
    v_Color = color;
    v_TexCoords = texCoords;
    v_TexIndex = texIndex;

    gl_Position = u_ViewProjection * vec4(pos, 1.0);
}

#type fragment
#version 460 core
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Textures[int(v_TexIndex)], v_TexCoords).r);
    color = v_Color * sampled;
}