//
// Created by asuka on 21.02.2024.
//

#include <GL/glu.h>
#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float viewingAngle,float aspect,float near,float far,
                                     float posX, float posY, float posZ,
                                     float lookX, float lookY,float lookZ,
                                     float upX, float upY, float upZ) {

    posCamX = posX;
    posCamY = posY;
    posCamZ = posZ;

    posLookX = lookX;
    posLookY = lookY;
    posLookZ = lookZ;

    this->upX = upX;
    this->upY = upY;
    this->upZ = upZ;

    angle = viewingAngle;
    this->aspect = aspect;
    this->near = near;
    this->far = far;

}

void PerspectiveCamera::render() {
    projectionMatrix = glm::perspective(angle, aspect, near, far);
    glm::vec3 cameraPosition(posCamX, posCamY, posCamZ);
    glm::vec3 targetPosition(posLookX, posLookY, posLookZ);
    glm::vec3 upVector(upX, upY, upZ);
    viewMatrix = glm::lookAt(cameraPosition, targetPosition, upVector);
}

PerspectiveCamera::PerspectiveCamera(float viewingAngle, float aspect, float near, float far, std::vector<float> pos,
                                     std::vector<float> look, std::vector<float> up) {

    posCamX = pos[0];
    posCamY = pos[1];
    posCamZ = pos[2];

    posLookX = look[0];
    posLookY = look[1];
    posLookZ = look[2];

    upX = up[0];
    upY = up[1];
    upZ = up[2];

    angle = viewingAngle;
    this->aspect = aspect;
    this->near = near;
    this->far = far;
}

[[maybe_unused]] void PerspectiveCamera::setLookAt(float lookX, float lookY, float lookZ) {
    posLookX = lookX;
    posLookY = lookY;
    posLookZ = lookZ;
}

[[maybe_unused]] void PerspectiveCamera::setLookAt(std::vector<float> look) {
    posLookX = look[0];
    posLookY = look[1];
    posLookZ = look[2];}

[[maybe_unused]] void PerspectiveCamera::setPosition(float posX, float posY, float posZ) {
    posCamX = posX;
    posCamY = posY;
    posCamZ = posZ;
}

[[maybe_unused]] void PerspectiveCamera::setPosition(std::vector<float> pos) {
    posCamX = pos[0];
    posCamY = pos[1];
    posCamZ = pos[2];
}

[[maybe_unused]] void PerspectiveCamera::setUp(float upX, float upY, float upZ) {
    this->upX = upX;
    this->upY = upY;
    this->upZ = upZ;
}

[[maybe_unused]] void PerspectiveCamera::setUp(std::vector<float> up) {
    upX = up[0];
    upY = up[1];
    upZ = up[2];
}

PerspectiveCamera::PerspectiveCamera() {}

glm::mat4 PerspectiveCamera::getViewMatrix() {
    return viewMatrix;
}

glm::mat4 PerspectiveCamera::getProjectionMatrix() {
    return projectionMatrix;
}

glm::vec3 PerspectiveCamera::getPosition() {
    return {posCamX,posCamY,posCamZ};
}

