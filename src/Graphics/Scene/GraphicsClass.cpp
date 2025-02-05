//
// Created by asuka on 13.05.2024.
//

#include <GL/glew.h>
#include <cmath>
#include <thread>
#include <filesystem>
#include "GraphicsClass.h"
#include "../Objects/Loaders/ObjLoader.h"
#include "../../Controller.h"
#include "../Math/Functionality.h"

void GraphicsClass::onCreate() {
    ConfigLoader graphics = Controller::config.getSubConfig("graphics");


    std::string model3DPath = graphics.getSubConfig("model3D").getString("path");
    std::string skeleton3DPath = graphics.getSubConfig("model3D").getString("skeleton");
    graphics.finalize();
    model = ObjLoader::load(model3DPath);
    model.loadSkeleton(skeleton3DPath);
    cam = PerspectiveCamera(60.0, 1.0, 0.1, 100.0,
                            0.0, 0.0, 0.0,
                            0.0, 7.985820/2, 0.0,
                            0.0, 1.0, 0.0);
    position = {0,7.98582,8};
}

void GraphicsClass::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    updateCam();
    cam.render();
    cam.setPosition(position.x,position.y,position.z);
    model.render(cam);
    glDisable(GL_DEPTH_TEST);
}
void GraphicsClass::updateCam() {
    degree += 0.5;
    if(degree >= 360){degree = 0;}
    position.x = 8*sinf(degree/k);
    position.z = 8*cosf(degree/k);
    model.lookAt(position);
}

void GraphicsClass::onExit() {
    model.destroy();
}

