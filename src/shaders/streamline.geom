#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

out float fMagnitude;
uniform int maxPointNum;

in VS_OUT {
    float magnitude;
    float widthRatio;
} gs_in[];

void main() {    
    vec4 lineDir = gl_in[0].gl_Position - gl_in[1].gl_Position;
    lineDir = normalize(lineDir);
    
    vec4 OrthLineDir = vec4(-lineDir.y, lineDir.x, 0.0f, 0.0f);

    /*
        P1--------P2
         \        /
          \      /
           P3--P4
    */
    float width = 0.002;
    // P1
    fMagnitude = gs_in[0].magnitude; 
    gl_Position = gl_in[0].gl_Position + OrthLineDir * gs_in[0].widthRatio * width; 
    EmitVertex();

    // P2
    fMagnitude = gs_in[0].magnitude;
    gl_Position = gl_in[0].gl_Position - OrthLineDir * gs_in[0].widthRatio * width; 
    EmitVertex();
    
    // P3
    fMagnitude = gs_in[1].magnitude;
    gl_Position = gl_in[1].gl_Position + OrthLineDir * gs_in[1].widthRatio * width; 
    EmitVertex();

    // P4
    fMagnitude = gs_in[1].magnitude;
    gl_Position = gl_in[1].gl_Position - OrthLineDir * gs_in[1].widthRatio * width; 
    EmitVertex();

    EndPrimitive();
}   