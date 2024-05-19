#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float aMagnitude;
layout (location = 2) in float aWidthRatio;
// layout (location = 3) in float aAlphaRatio;

uniform mat4 view;
uniform mat4 projection;
uniform float widthRatioClamp_m;
uniform float widthRatioClamp_M;

// out float magnitude;

out VS_OUT {
    float magnitude;
    float widthRatio;
    float alphaRatio;
} vs_out;

void main()
{
    gl_Position = projection * view * vec4(aPos.x, aPos.y, 0.0, 1.0);   
    vs_out.magnitude = aMagnitude;  
    float wr = clamp(aWidthRatio,widthRatioClamp_m,widthRatioClamp_M);
    vs_out.widthRatio = wr;
    vs_out.alphaRatio = aWidthRatio;
}
