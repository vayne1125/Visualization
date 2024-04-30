#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTextureCord;


uniform mat4 model;
uniform mat4 fixedRY;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos; 
out vec3 textureCord;
out mat3 inverseModel;
void main()
{
    mat4 finalModel = fixedRY * model;
    fragPos = vec3(finalModel * vec4(aPos, 1.0));

    gl_Position = projection * view * finalModel * vec4(aPos, 1.0);
    textureCord = aTextureCord; 

    inverseModel = mat3(transpose(inverse(finalModel)));                                                              
}
