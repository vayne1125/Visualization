#version 330 core
out vec4 fragColor; 
in float magnitude;

uniform float maxMagnitude;
uniform float minMagnitude;
uniform sampler1D texture1d;

void main(){    
    float val = (magnitude-minMagnitude)/(maxMagnitude-minMagnitude);
    fragColor = texture(texture1d,val);
}