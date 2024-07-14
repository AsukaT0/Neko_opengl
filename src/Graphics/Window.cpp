//
// Created by asuka on 20.02.2024.
//

#include "Window.h"
#include "../Controller.h"
#include <cstdio>

#define GLEW_STATIC

#include <utility>
#include <thread>
#include <future>


void Windower::createWindow(Application *application) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }
    windowSetups();
    window = glfwCreateWindow(width, height, "Мирай", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW mainWindows\n");
        glfwTerminate();
        return;
    }

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return;}
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glfwMakeContextCurrent(window);
    currAplic = application;
    while (!glfwWindowShouldClose(window)) {
        if (!isSetups&&currAplic != nullptr) {
            currAplic->onCreate();
            isSetups = true;}
        frameLim();
        if(killFlag)return;}
    invalidate();
    glfwTerminate();
    return;
}


void Windower::reshape([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 100.0);
}

void Windower::windowSizeCallback(GLFWwindow *window, int width, int height) { reshape(window, width, height); }

Windower::~Windower() { glfwTerminate(); }

void Windower::frameLim() {
    auto frameStart = std::chrono::high_resolution_clock::now();
    if (currAplic != nullptr) {
        currAplic->onRender();
        glFlush();}
    glfwPollEvents();
    auto frameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> frameDuration = frameEnd - frameStart;
    currentFrametic = std::max(frameDuration.count(),TARGET_FRAME_TIME);
    std::this_thread::sleep_for(std::chrono::duration<double>(TARGET_FRAME_TIME - frameDuration.count()));
}


void Windower::windowSetups() {
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
}

[[maybe_unused]] int Windower::getHeight() {
    return height;
}

[[maybe_unused]] int Windower::getWidth() {
    return width;
}

Windower::Windower() {
    Controller::mainWindows = this;
}

void Windower::onExit() {
    killFlag = true;
}

float Windower::getFramerate() const{
    return currentFrametic;
}

void Windower::invalidate() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    currAplic->onExit();
    if (currAplic != nullptr) {
        delete currAplic;
    }
}
