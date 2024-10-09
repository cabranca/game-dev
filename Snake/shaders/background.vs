#version 330 core

layout (location = 0) in vec2 vertexPosition; // <vec2 pos, vec2 texCoords>
layout (location = 1) in vec4 vertexColor;
out vec2 fragmentPosition;
out vec4 fragmentColor;

void main()
{
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
    fragmentPosition = vertexPosition;
    fragmentColor = vertexColor;
}