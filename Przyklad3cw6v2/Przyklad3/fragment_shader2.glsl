#version 330

out vec4 color;
//in vec3 outcol;
in vec2 UV;

uniform sampler2D myTexture;

void main()
{
    color  = texture(myTexture, UV);
} 
