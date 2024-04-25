#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aGradient;

uniform mat4 model;
uniform mat4 fixedRY;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 clipNormal;

out vec3 gradient;
out vec3 fragPos; 
// out vec3 lightPos; 
out float clipVal;

void main()
{
    mat4 finalModel = fixedRY * model;
    fragPos = vec3(finalModel * vec4(aPos, 1.0));
    gradient = mat3(transpose(inverse(finalModel))) * aGradient; 

    vec3 clipNorm = normalize(vec3(clipNormal));
    clipVal = dot(vec3(model * vec4(aPos, 1.0)), clipNorm) + clipNormal.w;

    gl_Position = projection * view * finalModel * vec4(aPos, 1.0);
}