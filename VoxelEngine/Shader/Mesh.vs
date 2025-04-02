#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

uniform mat4 transform;
uniform mat4 viewProj;

out vec3 ourColor;
out vec2 texCoord;
void main()
{
	gl_Position = viewProj * transform * vec4(aPos, 1.0);
	ourColor = aColor;
	texCoord = aTex;
}