#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 camMatrix;

void main()
{
    gl_Position = modelMatrix * camMatrix * vec4(position, 1.0);
    Normal = mat3(transpose(inverse(modelMatrix))) * normal;
    FragPos = vec3(modelMatrix * vec4(position, 1.0));
    texCoord = aTexCoord;
}