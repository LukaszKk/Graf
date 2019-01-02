#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 offset;

void main()
{
    gl_Position = vec4( aPos.x + offset.x*0.5, aPos.y + offset.y*0.5, aPos.z, 1.0);
}