#version 330 core
out vec4 fragColor; 

in float flabel;

void main(){    
    if(flabel == 1.0){
        fragColor = vec4(1, 122/255.0, 189/255.0, 0.8);
    }else if(flabel == 0.0){
        fragColor = vec4(0,1,238/255.0,0.8);
    }else fragColor = vec4(1,1,1,0.8);
}