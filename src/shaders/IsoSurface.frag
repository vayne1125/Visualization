#version 330 core
out vec4 fragColor; 

uniform vec4 objectColor;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform bool enableCliped;

in float clipVal;
in vec3 gradient;
in vec3 fragPos;

void main(){

    if(enableCliped && abs(clipVal) > 1.0){
        discard;
    }

    if(clipVal > 0.0){
        discard;
    }
    vec3 lightColor = vec3(1,1,1);

    vec3 norm = normalize(gradient);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos); 

    if (dot(norm, viewDir) < 0)
	{
        norm = -norm;
    }

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor; 

    // diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    // vec4 result = clamp(vec4(ambient + diffuse + specular,1), vec4(0.0), vec4(1.0)) * objectColor;;
    vec4 result = clamp(vec4(ambient + diffuse,1), vec4(0.0), vec4(1.0)) * objectColor;;
    //vec4(ambient + diffuse + specular,1) * objectColor;
    fragColor = result;
    // vec4 result = vec4(ambient + diffuse, 1) * objectColor;
    // fragColor = result;
}