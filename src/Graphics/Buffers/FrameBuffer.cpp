//
// Created by asuka on 30.04.2024.
//

#include "FrameBuffer.h"
#include "../../Controller.h"

FrameBuffer::FrameBuffer() {
}

FrameBuffer::FrameBuffer(int width,int height) {
    frame = Texture(height,width);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame.getTextureLink(), 0);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);

    GLenum attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Failed to initialize framebuffer\n");
    }

// Отвязка фреймбуфера
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
GLuint FrameBuffer::getBuffer() {
    return framebuffer;
}

Texture FrameBuffer::getTexture() {
    return frame;
}

void FrameBuffer::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, Controller::mainWindows->getWidth(), Controller::mainWindows->getHeight());
}

void FrameBuffer::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Controller::mainWindows->getWidth(), Controller::mainWindows->getHeight());
}

void FrameBuffer::init() {
    frame = Texture();
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame.getTextureLink(), 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "Failed to initialize framebuffer");
    }

    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Controller::mainWindows->getWidth(), Controller::mainWindows->getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FrameBuffer::destroy() {
    frame.destroy();
    if (framebuffer != 0) {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;}
    if (depthBuffer != 0) {
        glDeleteRenderbuffers(1, &depthBuffer);
        depthBuffer = 0;}
}

GLuint FrameBuffer::getDepthTexture() {
    return depthTexture;
}

GLuint FrameBuffer::getNormalTexture() {
    return normalTexture;
}
