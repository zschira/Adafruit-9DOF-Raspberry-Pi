#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.5;
    vec4 color = texture(texture_diffuse1, TexCoords);
    color.a = 1;
    // ambient
    vec4 ambient = ambientStrength * color;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = vec3(-0.5, -0.5, -0.3);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * color;
    
    // specular
    float specularStrength = 0.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = specularStrength * spec * color;  
        
    FragColor = vec4(ambient + diffuse + specular);
    FragColor.a = 1;
} 