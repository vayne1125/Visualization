#version 330 core
out vec4 fragColor; 

in float fMagnitude;
in float fAlphaRatio;

uniform float maxMagnitude;
uniform float minMagnitude;
uniform sampler1D texture1d;

void main(){    
    float val = (fMagnitude-minMagnitude)/(maxMagnitude-minMagnitude);
    fragColor = texture(texture1d,val);
    fragColor.a = fAlphaRatio;
}