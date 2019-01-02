#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inCol;
layout (location = 2) in vec3 inOffset;

out vec3 color;
out vec3 offset;

void main()
{
	color = inCol;
    gl_Position = vec4( aPos, 1.0);
}