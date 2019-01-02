#version 330

in vec2 UV;
out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
    color  = texture( myTextureSampler, UV );
} 
