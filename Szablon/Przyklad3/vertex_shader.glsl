#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 vtexcoord0;
layout (location = 3) in float onOff;


uniform mat4 MVP;
out vec2 texcoord0;
out vec3 Normal;
out vec3 FragPos;
out float offOn;


void main()
{
	offOn = onOff;
    gl_Position = MVP*vec4(aPos, 1.0);
	texcoord0 = vtexcoord0;
	FragPos = aPos;
    Normal = normalize( aNormal); 
}