#version 330 core
out vec4 fragcolor; 

uniform sampler3D texture3d;
uniform sampler1D texture1d;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform bool openPhong;
uniform float gap;

in vec3 fragPos;
in vec3 textureCord;

bool outside(vec3 texCoord){
    if(texCoord.x < 0.0f || texCoord.x > 1.0f) return true;
    if(texCoord.y < 0.0f || texCoord.y > 1.0f) return true;
    if(texCoord.z < 0.0f || texCoord.z > 1.0f) return true;
    return false;
}

vec3 phong(vec3 position, vec3 gradient, vec3 color){
    
    for(int i = 0; i<3; i++)
        gradient[i] = 2.0 * gradient[i] - 1.0;
    
    vec3 lightcolor = vec3(1,1,1);
    
    vec3 norm = normalize(gradient);
    vec3 lightDir = normalize(lightPos - position);
    vec3 viewDir = normalize(viewPos - position); 

    if (dot(norm, viewDir) < 0)
    {
        norm = -norm;
    }

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightcolor; 

    // diffuse 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightcolor;

    // specular
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightcolor; 

    vec3 result = clamp(vec3(ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * color;
    return result;
}
void main()
{
    vec3 raydir = normalize(vec3(0,0,0)-viewPos);

    float T = 0.0f; //accumulate Opacities
    vec3 position = fragPos;
    vec3 texCoord = textureCord;
    vec3 color = vec3(0.0f);
    
    while(true){   
        vec4 texel0 = texture(texture3d, texCoord);
        vec4 texel1 = texture(texture1d, texel0.a); 
        vec3 nowColor = texel1.rgb;
        vec3 phongColor = nowColor;

        if(openPhong) phongColor = phong(position, texel0.rgb, nowColor);

        color = color + phongColor * texel1.a * (1 - T);
        T = T + (1 - T) * texel1.a;
        position = position + raydir * gap;
        texCoord = textureCord + (position - fragPos) / 255.0f;
        if(outside(texCoord)) break;
        if(T >= 1.0) break;
    }
    fragcolor = vec4(color, min(T, 1.0f));
    // fragcolor = vec4(1,0,0,1);
}
