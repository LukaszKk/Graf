#version 330

out vec4 FragColor;
in vec2 texcoord0;
uniform sampler2D tex0;
in float offOn;

void main()
{
	if( offOn == 0 )
		gl_FragColor = vec4( 1, 0, 1, 1 );
	else
		gl_FragColor = texture( tex0, texcoord0 );
	
	//lub
	//FragColor = texture( tex0, texcoord0 );
	
} 
