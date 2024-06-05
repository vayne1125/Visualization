#version 330 core
out vec4 fragColor; 

uniform vec4 color0;
uniform vec4 color1;

in float flabel;

void main(){    
    if(flabel == 0.0){
        fragColor = color0;
    }else if(flabel == 1.0){
        fragColor = color1;
    }else fragColor = vec4(1,0,0,1);
}