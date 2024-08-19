//
// Created by asuka on 30.04.2024.
//

#ifndef MIRAAI_TEXTURE_H
#define MIRAAI_TEXTURE_H


#include <GL/glew.h>
#include <functional>
#include "../../Shaders/ShaderProgram.h"

class Texture {
private:
    GLuint tex = 0;
    std::function<void()> doing = [] {};
    bool isSetupped = false, renderSetup = false;
    std::string imgPath;

public:
    Texture();
    Texture(int height, int width);
    void render();

    void loadImg(const std::string &path);

    void render(int width, int height);

    GLuint &getTextureLink() { return tex; }

    bool isLoaded();

    void destroy();
    ~Texture();
};


#endif //MIRAAI_TEXTURE_H
