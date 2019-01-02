#version 330 core 

in vec3 outCol;
out vec4 FragColor;

void main()
{
    FragColor = vec4( outCol, 1.0f );
} 
