#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 viewProj;

out vec3 color;
out vec3 normal;
void main()
{
	gl_Position = viewProj * transform * vec4(aPos, 1.0);
	if (aNormal == vec3(0,1,0) || aNormal == vec3(0,-1,0))
		color = vec3(0.8, 0.7, 0.8);
	else
		color = vec3(0.2, 0.5, 0.7);
	normal = aNormal;
}