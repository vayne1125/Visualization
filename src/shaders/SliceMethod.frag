#version 330 core
out vec4 fragColor; 

uniform sampler3D texture3d;
uniform sampler1D texture1d;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform float maxMag;
uniform float minMag;
uniform bool openPhong;
// uniform int sliceNum;

in vec3 fragPos;
in vec3 textureCord;
in mat3 inverseModel;
void main(){    
    float val = texture(texture3d,textureCord).a;
    if(val <= 0.1)
        discard;
    
    fragColor = vec4(0,0,0,0);
    float alpha = (length(texture(texture3d,textureCord).xyz) - minMag)/(maxMag-minMag);
    vec4 color = texture(texture1d,val);
    // color.a = color.a * (512.0/sliceNum);
    if(alpha > 0.8)
        color.a = color.a * (1+3*alpha);
    else
        color.a = color.a * (1 - alpha);
    if(!openPhong)    
        fragColor = color;
    else{
        vec3 gradient = texture(texture3d,textureCord).rgb;

        for(int i = 0; i<3; i++)
            gradient[i] = 2.0 * gradient[i] - 1.0;
        
        gradient = inverseModel * gradient;

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

        vec3 result = clamp(vec3(ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * color.rgb;
        fragColor = vec4(result,color.a);
    }
    // fragColor = color;
}