#version 330 core
out vec4 fragColor; 

uniform sampler3D texture3d;
uniform sampler1D texture1d;
// uniform vec3 objectHSVColor;
// uniform vec3 viewPos;
// uniform vec3 lightPos;
uniform bool enableCliped;
uniform float maxMag;
uniform float minMag;

in float clipVal;
in vec3 fragPos;
in vec3 textureCord;
void main(){
    if(enableCliped && abs(clipVal) > 1.0){
        discard;
    }
    
    if(clipVal > 0.0){
        discard;
    }
    
    float val = texture(texture3d,textureCord).a;
    if(val <= 0.1)
        discard;
    
    fragColor = vec4(0,0,0,0);
    float alpha = (length(texture(texture3d,textureCord).xyz) - minMag)/(maxMag-minMag);
    vec4 color = texture(texture1d,val);
    
    // fragColor = color;
    // gradient = texture(texture3d,textureCord).rgb;
    
    // for(int i = 0; i<3; i++)
    //     gradient[i] = 2.0 * gradient[i] - 1.0;
    
    fragColor = color;

    // vec3 lightColor = vec3(1,1,1);

    // vec3 norm = normalize(gradient);
    // vec3 lightDir = normalize(lightPos - fragPos);
    // vec3 viewDir = normalize(viewPos - fragPos); 

    // if (dot(norm, viewDir) < 0)
	// {
    //     norm = -norm;
    // }

    // // ambient
    // float ambientStrength = 0.1;
    // vec3 ambient = ambientStrength * lightColor; 

    // // diffuse 
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;

    // // specular
    // float specularStrength = 0.5;
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor; 

    // vec4 result = clamp(vec3(ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * color.rgb;
    // fragColor = vec4(result,color.a);
}