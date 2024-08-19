//
// Created by asuka on 30.04.2024.
//

#include "Texture.h"
#include "../../../Controller.h"
#include <SOIL/SOIL.h>

void Texture::render() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);glTexCoord2f(0.0f, 0.0f);glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex2f(-1.0f, 1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}


void Texture::render(int width, int height) {
    float posX = ((Controller::mainWindows->getWidth() - width)/2.0 / Controller::mainWindows->getWidth());
    float posY = ((Controller::mainWindows->getHeight() - height)/2.0 / Controller::mainWindows->getHeight());
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(posX, posY);
    glTexCoord2f(1.0f, 0);
    glVertex2f(-posX, posY);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(-posX, -posY);
    glTexCoord2f(0, 1.0f);
    glVertex2f(posX, - posY);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

Texture::Texture(int height, int width) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    isSetupped = true;
    if (!imgPath.empty()) {
        int width2, height2;
        unsigned char *image = SOIL_load_image(&imgPath[0], &width2, &height2, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
}

Texture::Texture() {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Controller::mainWindows->getWidth(), Controller::mainWindows->getHeight(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    isSetupped = true;
};

void Texture::loadImg(const std::string &path) {
    imgPath = path;
    if (!isSetupped) {
        return;
    }

    int width, height;
    unsigned char *image = SOIL_load_image(path.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);

    if (image == nullptr) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
}

bool Texture::isLoaded() {
    if(!imgPath.empty()&&tex == 0){
        loadImg(imgPath);
    }
    return !imgPath.empty();
}

void Texture::destroy() {
    glDeleteTextures(1, &tex);
    doing = [] {};
    imgPath.clear();
}

Texture::~Texture() {
//    glDeleteTextures(1,&tex);
}


