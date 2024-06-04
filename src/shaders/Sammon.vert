#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float alabel;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    float glabel;
} vs_out;

void main()
{
    gl_Position = projection * view * vec4(aPos.x, aPos.y, 0.0, 1.0);  
    vs_out.glabel = alabel; 
}
