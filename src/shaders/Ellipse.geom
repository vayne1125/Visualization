#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

out float flabel;
uniform float widthRatio;

in VS_OUT {
    float glabel;
} gs_in[];

void main() {    
    vec4 lineDir = gl_in[0].gl_Position - gl_in[1].gl_Position;
    lineDir = normalize(lineDir);
    
    vec4 OrthLineDir = vec4(-lineDir.y, lineDir.x, 0.0f, 0.0f);

    /*
        P1--------P2
        |          |
        |          |
        |          |
        P3--------P4
    */
    float width = 0.0075;

    // P1
    flabel = gs_in[0].glabel;
    gl_Position = gl_in[0].gl_Position + OrthLineDir * widthRatio * width; 
    EmitVertex();

    // P2
    flabel = gs_in[0].glabel;
    gl_Position = gl_in[0].gl_Position - OrthLineDir * widthRatio * width; 
    EmitVertex();
    
    // P3
    flabel = gs_in[1].glabel;
    gl_Position = gl_in[1].gl_Position + OrthLineDir * widthRatio * width; 
    EmitVertex();

    // P4
    flabel = gs_in[1].glabel;
    gl_Position = gl_in[1].gl_Position - OrthLineDir * widthRatio * width; 
    EmitVertex();

    EndPrimitive();
}   