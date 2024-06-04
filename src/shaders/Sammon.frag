#version 330 core
out vec4 fragColor; 

in float flabel;

void main(){    
    if(flabel == 1.0){
        fragColor = vec4(1,0,0,1);
    }else if(flabel == 0.0){
        fragColor = vec4(0,0,1,1);
    }else fragColor = vec4(1,1,1,1);
}