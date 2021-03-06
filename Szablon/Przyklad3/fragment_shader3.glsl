#version 330

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
vec3 viewPos; 

in float offOn;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    viewPos[0] = 300;
    viewPos[1] = 100;
    viewPos[2] = 300;
    
	// specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
        
    vec3 result = (ambient + diffuse + specular) * objectColor;

	if( offOn == 0 )
		FragColor = vec4( 1, 0.5, 0, 1 );
	else
		FragColor = vec4(result, 1.0);
}