#version 330 core

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float time;

void main()
{
    color = vec4(fragmentColor.r * (cos(fragmentPosition.x * 4.0f + time) + 1.0f) * 0.5f,
                 fragmentColor.g * (cos(fragmentPosition.x * 8.0f + time) + 1.0f) * 0.5f,
                 fragmentColor.b * (cos(fragmentPosition.x * 2.0f + time) + 1.0f) * 0.5f,
                 fragmentColor.a);
}