//
// Created by asuka on 13.04.2024.
//

#include "DiffusionLight.h"

[[maybe_unused]] DiffusionLight::DiffusionLight(float x, float y, float z) {
    position = Vector3D{x,y,z};
}

[[maybe_unused]] DiffusionLight::DiffusionLight(Vector3D position) {
    this->position = position;
}

[[maybe_unused]] Vector3D DiffusionLight::getPosition() {
    return position;
}
