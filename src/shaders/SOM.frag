#version 330 core
out vec4 fragColor; 

in vec3 fragPos;
in vec3 gradient;
in vec2 texturecord;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform int renderTo;
uniform sampler2D texture2d;

void main(){    
    // fragColor = vec4(1,1,1,1);

    vec3 color = texture(texture2d,texturecord).rgb;
    // vec3 color = vec3(1,0,0);
    if(renderTo == 0){  // lines
        fragColor = vec4(1,1,1,1);
    }else if(renderTo == 1){  // surfaces
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

        vec3 result = clamp(vec3(ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * color;
        // vec3 result = clamp(vec3(ambient + diffuse), vec3(0.0), vec3(1.0)) * color;
        fragColor = vec4(result,1);
    }

}