//
// Created by asuka on 13.05.2024.
//

#ifndef MIRAI_GRAPHICSCLASS_H
#define MIRAI_GRAPHICSCLASS_H


#include <cmath>
#include "../Graphics/Math/Structs.h"
#include "../Graphics/Objects/gl3D/Model/Model3D/Model.h"
#include "../Graphics/Objects/gl3D/Camera/PerspectiveCamera.h"
#include "Application.h"
#include "../Graphics/Objects/gl3D/Model/Model3D/HumaModel.h"

class GraphicsClass : public Application{
private:
    float degree = 0;
    float k = 1/M_PI*180;
    Vertex position{0, 0, 0};
    HumaModel model{};
    PerspectiveCamera cam{};
    void updateCam();
public:
    void onCreate() override;
    void onRender() override;
    void onExit() override;
};


#endif //MIRAI_GRAPHICSCLASS_H
