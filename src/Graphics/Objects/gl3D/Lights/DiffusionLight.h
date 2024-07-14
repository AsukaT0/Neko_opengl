//
// Created by asuka on 13.04.2024.
//

#ifndef MIRAAI_DIFFUSIONLIGHT_H
#define MIRAAI_DIFFUSIONLIGHT_H


#include "../../../Math/Structs.h"

class DiffusionLight {
private:
    Vector3D position{0,100,0};
public:
    [[maybe_unused]] explicit DiffusionLight(Vector3D position);
    [[maybe_unused]] DiffusionLight(float x,float y,float z);
    [[maybe_unused]] Vector3D getPosition();
};


#endif //MIRAAI_DIFFUSIONLIGHT_H
