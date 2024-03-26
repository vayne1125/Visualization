#version 330 core
out vec4 fragColor; 

uniform vec3 objectHSVColor;
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

    // HSV to RGB
    float h =  objectHSVColor.x;
    float s =  objectHSVColor.y;
    float l =  objectHSVColor.z;

    float q = l + s - (l*s);
    float p = 2 * l - q;
    float hk = h/360.0;
    // float tpR = hk + 1/3.0, tpG = hk, tpB = hk-1/3.0;
    vec3 tpRGB = vec3(hk + 1/3.0, hk, hk-1/3.0), RGB;

    for(int i=0;i<3;i++){
        if(tpRGB[i] < 0) tpRGB[i] += 1.0;
        else if(tpRGB[i] > 1) tpRGB[i] -= 1.0; 
    }
    for(int i=0;i<3;i++){
        if(tpRGB[i] < 1/6.0) RGB[i] = p+(q-p)*6*tpRGB[i];
        else if(1/6.0 <= tpRGB[i] && tpRGB[i] < 1/2.0) RGB[i] = q;
        else if(1/2.0 <= tpRGB[i] && tpRGB[i] < 2/3.0) RGB[i] = p + (q-p)*6*(2/3.0 - tpRGB[i]);
        else RGB[i] = p;
    }

    // vec3 objectRGBColor = vec3();
    vec3 result = clamp(vec3(ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * RGB;
    //vec4(ambient + diffuse + specular,1) * objectColor;
    fragColor = vec4(result,1);
    // vec4 result = vec4(ambient + diffuse, 1) * objectColor;
    // fragColor = result;
}