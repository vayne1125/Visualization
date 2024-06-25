#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aGradient;
layout (location = 2) in vec2 aTexturecord;

uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 gradient;
out vec2 texturecord;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);   

    fragPos = aPos;
    gradient = aGradient; 
    texturecord = aTexturecord;
}
