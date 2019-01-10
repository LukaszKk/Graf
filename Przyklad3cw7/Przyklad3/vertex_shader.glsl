#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vtexcoord0;
layout (location = 2) in vec3 aNormal;


uniform mat4 MVP;
out vec2 texcoord0;
out vec3 Normal;
out vec3 FragPos;


void main()
{
    gl_Position = MVP*vec4(aPos, 1.0);
	texcoord0 = vtexcoord0;
	FragPos = aPos;
    Normal = normalize( aNormal); 
}