#version 330 core 

in vec3 color;
in vec3 offset;
out vec4 FragColor;

void main()
{
    FragColor = vec4( color.x + offset.x, color.y + offset.y, color.z + offset.z, 1.0f);
} 
