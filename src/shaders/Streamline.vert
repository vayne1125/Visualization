#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float aMagnitude;
layout (location = 2) in float aWidthRatio;

uniform mat4 view;
uniform mat4 projection;

// out float magnitude;

out VS_OUT {
    float magnitude;
    float widthRatio;
} vs_out;

void main()
{
    gl_Position = projection * view * vec4(aPos.x, aPos.y, 0.0, 1.0);   
    vs_out.magnitude = aMagnitude;  
    vs_out.widthRatio = aWidthRatio;
}
