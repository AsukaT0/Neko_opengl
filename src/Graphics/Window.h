//
// Created by asuka on 20.02.2024.
//

#ifndef MIRAAI_WINDOW_H
#define MIRAAI_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <queue>
#include "Objects/gl3D/Model/Model3D/Model.h"
#include "Scene/Application.h"

class Windower {
private:
    bool isSetups = false,killFlag = false;
    void frameLim();
    void windowSetups();
    int width = 800;
    int height = 800;
    Application *currAplic = nullptr;
    float currentFrametic = 0;
    void invalidate();
public:
    GLFWwindow* window;
    constexpr static double TARGET_FRAME_TIME = 1.0 / 60.0;
    float getFramerate() const;

    void createWindow(Application* application);
    Windower();
    static void reshape([[maybe_unused]] GLFWwindow* window, int width, int height);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    ~Windower();
    [[maybe_unused]] [[nodiscard]]
    int getHeight();
    [[maybe_unused]] [[nodiscard]]
    int getWidth();
    void onExit();
};


#endif //MIRAAI_WINDOW_H
