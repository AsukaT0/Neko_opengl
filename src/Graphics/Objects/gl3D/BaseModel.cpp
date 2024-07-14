//
// Created by asuka on 24.02.2024.
//

#include <valarray>
#include <iostream>
#include <glm/trigonometric.hpp>
#include "BaseModel.h"

[[maybe_unused]] void BaseModel::setGlobalPosition(std::vector<float> pos) {
    setGlobalPosition(pos[0],pos[1],pos[2]);}
[[maybe_unused]] void BaseModel::setGlobalPosition(Vector3D pos) {
    setGlobalPosition(pos.x,pos.y,pos.z);}
[[maybe_unused]] void BaseModel::setGlobalPosition(float x, float y, float z) {
    modelPosXGlobal = x;
    modelPosYGlobal = y;
    modelPosZGlobal = z;
}
void BaseModel::setGlobalPositionX(float x) {modelPosXGlobal = x;}
void BaseModel::setGlobalPositionY(float y) {modelPosYGlobal = y;}
void BaseModel::setGlobalPositionZ(float z) {modelPosZGlobal = z;}


void BaseModel::setLocalRotation(std::vector<float> rotation) {
    setLocalRotation(rotation[0],rotation[1],rotation[2]);}
void BaseModel::setLocalRotation(float x, float y, float z) {
    modelRotXLocal = glm::radians(x);
    modelRotYLocal = glm::radians(y);
    modelRotZLocal = glm::radians(z);

    apudater();
}
void BaseModel::setLocalRotationX(float x) {modelRotXLocal = x;apudater();}
void BaseModel::setLocalRotationY(float y) {modelRotYLocal = glm::radians(y);apudater();}
void BaseModel::setLocalRotationZ(float z) {modelRotZLocal = z;apudater();}
void BaseModel::setLocalRotationRad(std::vector<float> rotation) {
    setLocalRotationRad(rotation[0],rotation[1],rotation[2]);
}
void BaseModel::setLocalRotationRad(float x, float y, float z) {
    modelRotXLocal = x;
    modelRotYLocal = y;
    modelRotZLocal = z;
    apudater();}


void BaseModel::setGlobalRotation(std::vector<float> rotation) {
    setGlobalRotation(rotation[0],rotation[1],rotation[2]);}
void BaseModel::setGlobalRotation(float x, float y, float z) {
    modelRotXGlobal = glm::radians(x);
    modelRotYGlobal = glm::radians(y);
    modelRotZGlobal = glm::radians(z);}
void BaseModel::setGlobalRotationX(float x) {modelRotXGlobal = x;}
void BaseModel::setGlobalRotationY(float y) {modelRotYGlobal = y;}
void BaseModel::setGlobalRotationZ(float z) {modelRotZGlobal = z;}
void BaseModel::setGlobalRotationRad(std::vector<float> rotation) {
    setGlobalRotationRad(rotation[0],rotation[1],rotation[2]);}
void BaseModel::setGlobalRotationRad(float x, float y, float z) {
    modelRotXGlobal = x;
    modelRotYGlobal = y;
    modelRotZGlobal = z;}


void BaseModel::lookAt(float x,float y,float z) {
    float dx = x - modelPosXGlobal;
    float dz = z - modelPosZGlobal;
    float lengthSquared2 = dx * dx + dz * dz;
    if(dx == 0&&dz!=0){
        modelRotYGlobal = (dz < 0)?glm::radians(90.0):glm::radians(270.0);}
    else if(dx != 0&&dz==0){
        if(dx < 0)
            modelRotYGlobal = 180;}
    else if(lengthSquared2 == 0){
        std::cout << "Error:lenght = 0\n";}
    else {
        if (dx != tempLookAt.x && dz != tempLookAt.z) {
            float length = sqrt(lengthSquared2);
            float sinAlphaY = -dz / length;
            float alphaY = asin(sinAlphaY);
            modelRotYGlobal = (dx > 0.0f) ? alphaY : -alphaY + 3.141592;
            tempLookAt.x = dx;
            tempLookAt.z = dz;}}
    float dy = y - modelPosYGlobal;
    if(dy != tempLookAt.y) {
        float lengthSquared = lengthSquared2 + dy * dy;
        if(lengthSquared == 0){
            std::cout << "Error:lenght2 = 0\n";
            return;}
        float length = sqrt(lengthSquared);
        float sinAlphaH = dy / length;
        modelRotZGlobal = asin(sinAlphaH);
        tempLookAt.y = dy;}}

void BaseModel::lookAt(Vector3D  pos) {
    lookAt(pos.x,pos.y,pos.z);}
void BaseModel::lookAt(std::vector<float> pos) {
    lookAt(pos[0],pos[1],pos[2]);}


void BaseModel::setColor(std::vector<float> color) {
    r = color[0];g = color[1];b = color[2];a = color[3];
}
void BaseModel::setColor(float r, float g, float b, float a) {
    this->r = r;this->g = g;this->b = b;this->a = a;
}


[[maybe_unused]] void BaseModel::setLocalScale(std::vector<float> scale) {setLocalScale(scale[0],scale[1],scale[2]);}
[[maybe_unused]] void BaseModel::setLocalScale(float x, float y, float z) {
    modelScaleXLocal = x;
    modelScaleYLocal = y;
    modelScaleZLocal = z;}
[[maybe_unused]] void BaseModel::setLocalScaleX(float x) {modelScaleXLocal = x;}
void BaseModel::setLocalScaleY(float y) {modelScaleYLocal = y;}
void BaseModel::setLocalScaleZ(float z) {modelScaleZLocal = z;}


void BaseModel::apudater() {
    if(modelRotXLocal==0&&modelRotYLocal==0&&modelRotZLocal==0)return;
    modelDegree = std::max(modelRotXLocal,std::max(modelRotYLocal,modelRotZLocal));
    modelRotKX = modelRotXLocal/modelDegree;
    modelRotKY = modelRotYLocal/modelDegree;
    modelRotKZ = modelRotZLocal/modelDegree;
}

