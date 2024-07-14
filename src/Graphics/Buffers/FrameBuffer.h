//
// Created by asuka on 30.04.2024.
//

#ifndef MIRAAI_FRAMEBUFFER_H
#define MIRAAI_FRAMEBUFFER_H


#include <GL/glew.h>
#include "../Objects/gl2D/Texture.h"

class FrameBuffer {
private:
    GLuint
            framebuffer,
            depthBuffer;
    GLuint
        depthTexture,
        normalTexture;

    Texture frame;
public:
    FrameBuffer();
    FrameBuffer(int width,int height);
    void init();
    GLuint getBuffer();
    Texture getTexture();
    GLuint getDepthTexture();
    GLuint getNormalTexture();
    void begin();
    void end();
    void destroy();
};


#endif //MIRAAI_FRAMEBUFFER_H
