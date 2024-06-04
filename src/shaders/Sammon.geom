#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 180) out;

uniform float screenW;
uniform float screenH;
uniform float ballSizeRatio;

out float flabel;

in VS_OUT {
    float glabel;
} gs_in[];

void main() {    
    float PI = 3.1425926f;
    float rad = 0.02 * ballSizeRatio;
    float N = 180;
    
    for(int i=0;i<N/2;i++){
        float deg = i/N;
        flabel = gs_in[0].glabel;
        
        if(screenH > screenW){
            gl_Position = vec4(gl_in[0].gl_Position.x + rad  * cos(2*PI*deg), gl_in[0].gl_Position.y + rad * screenW / screenH * sin(2*PI*deg), 0, 1);        
            EmitVertex();

            deg = (N-i-1)/N;
            gl_Position = vec4(gl_in[0].gl_Position.x + rad  * cos(2*PI*deg), gl_in[0].gl_Position.y + rad * screenW / screenH * sin(2*PI*deg), 0, 1);
            EmitVertex();
        }else{
            gl_Position = vec4(gl_in[0].gl_Position.x + rad * screenH / screenW * cos(2*PI*deg), gl_in[0].gl_Position.y + rad * sin(2*PI*deg), 0, 1);        
            EmitVertex();

            deg = (N-i-1)/N;
            gl_Position = vec4(gl_in[0].gl_Position.x + rad * screenH / screenW * cos(2*PI*deg), gl_in[0].gl_Position.y + rad * sin(2*PI*deg), 0, 1);
            EmitVertex();
        } 
    }
    EndPrimitive();
}   