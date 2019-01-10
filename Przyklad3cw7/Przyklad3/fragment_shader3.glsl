#version 330

in vec2 texcoord0;
in vec3 Normal;  
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D tex0;

void main()
{

    gl_FragColor = vec4( 1, 0.5, 0, 1);
	
	//gl_FragColor = texture( tex0, texcoord0 );
	
	//lub
	//FragColor = texture( tex0, texcoord0 );
	
} 
