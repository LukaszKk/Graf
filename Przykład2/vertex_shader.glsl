#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 vColor;        // Coming in from your OpenGL program
layout (location = 2) in vec3 offset;

out vec4 color;        // Going to (and defined in) your fragment shader.


void main()
{
    gl_Position =  (aPos.x, aPos.y, aPos.z, 1.0);
    color = vColor;
}