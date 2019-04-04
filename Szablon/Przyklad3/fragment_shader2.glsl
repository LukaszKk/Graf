#version 330

out vec4 FragColor;
in vec2 texcoord0;
uniform sampler2D tex0;

in float offOn;

vec4 blend_text; 

void main()
{
	if( offOn == 0 )
	{
		gl_FragColor = vec4( 0, 0.5, 0, 1);
	}
	else
	{
		blend_text = texture( tex0, texcoord0 );
		if( blend_text[1] == 0 )
			blend_text[3] = 0.0;
		gl_FragColor = blend_text;
	}

	//lub
	//FragColor = texture( tex0, texcoord0 );
	
} 
