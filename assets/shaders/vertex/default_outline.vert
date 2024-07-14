#version 330 core
layout (location = 0) in vec3 position;

out vec3 FragColor;

uniform mat4 modelMatrix;
uniform mat4 camMatrix;

void main() {
    gl_Position = modelMatrix * camMatrix * vec4(position, 1.0);
    FragColor = vec3(0,0,0);
}