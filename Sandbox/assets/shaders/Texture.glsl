#type vertex
#version 330 core
			
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 u_viewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = tex;
	gl_Position = u_viewProjection * u_Transform * vec4(pos, 1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord);
}