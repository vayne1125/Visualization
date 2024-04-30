#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTextureCord;


uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos; 
out vec3 textureCord;
void main()
{
    fragPos = aPos;

    gl_Position = projection * view * vec4(aPos, 1.0);
    textureCord = aTextureCord; 
}
