#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourNormal;
out vec2 TexCoord;
out vec3 FragPos;
uniform mat4 VP;
uniform mat4 M;
void main()
{
    gl_Position = VP * M * vec4(aPos, 1.0);
    ourNormal = mat3(transpose(inverse(M))) * aNormal;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    FragPos = vec3(M * vec4(aPos, 1.0));
}