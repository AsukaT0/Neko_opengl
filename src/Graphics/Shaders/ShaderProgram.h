//
// Created by asuka on 21.02.2024.
//

#ifndef MIRAAI_SHADERPROGRAM_H
#define MIRAAI_SHADERPROGRAM_H


#include <string>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

class ShaderProgram {
private:
    std::string
            fragShader,
            vertShader;
    GLuint
            shaderProgram = 0;
public:
    ShaderProgram() = default;
    ShaderProgram(const std::string& vertPath, const std::string& fragPath);
    ~ShaderProgram();
    void readShaders(const std::string& vertPath,const std::string& fragPath);
    void setupShader();
    GLuint compileVertexShader();
    GLuint compileFragmentShader();
    void begin();
    void end();
    GLuint getUniPath(const GLchar *name);
    void destroy();
};


#endif //MIRAAI_SHADERPROGRAM_H
