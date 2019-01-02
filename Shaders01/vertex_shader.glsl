#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inCol;
out vec3 outCol;

void main()
{
	outCol = vec3( inCol );
    gl_Position = vec4( aPos, 1.0f );
}