#include <GL/glew.h>
#include <File/File.h>
#include "ShaderProgram.h"

GLuint ShaderProgram::compileVertexShader() {
    const char* vSh = vertShader.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vSh, nullptr);
    glCompileShader(vertexShader);
    return vertexShader;}

GLuint ShaderProgram::compileFragmentShader() {
    const char* fSh = fragShader.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fSh, nullptr);
    glCompileShader(fragmentShader);
    return fragmentShader;}

void ShaderProgram::setupShader() {
    GLint vertexShader = compileVertexShader();
    GLint fragmentShader = compileFragmentShader();
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    vertShader.clear();
    fragShader.clear();
}

ShaderProgram::ShaderProgram(const std::string& vertPath, const std::string& fragPath) {
    if(!File(fragPath).fileExists()&&File(vertPath).fileExists()){
        throw ("Шейдер по пути "+fragPath + " или " + vertPath + "не найден\n");
    }
    fragShader = File(fragPath).read();
    vertShader = File(vertPath).read();
    fragShader = fragShader.substr(0,fragShader.size()-1);
    vertShader = vertShader.substr(0,vertShader.size()-1);

}

void ShaderProgram::readShaders(const std::string& vertPath, const std::string& fragPath) {
    fragShader = File(fragPath).read();
    vertShader = File(vertPath).read();
    fragShader = fragShader.substr(0,fragShader.size()-1);
    vertShader = vertShader.substr(0,vertShader.size()-1);
}

void ShaderProgram::begin() {
    glUseProgram(shaderProgram);
}

void ShaderProgram::end() {
    glUseProgram(0);
}

GLuint ShaderProgram::getUniPath(const GLchar *name) {
    return glGetUniformLocation(shaderProgram, name);
}

ShaderProgram::~ShaderProgram() {
//    glDeleteProgram(shaderProgram);
}

void ShaderProgram::destroy() {
    glDeleteProgram(shaderProgram);
}
