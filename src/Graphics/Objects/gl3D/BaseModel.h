//
// Created by asuka on 24.02.2024.
//

#ifndef MIRAAI_BASEMODEL_H
#define MIRAAI_BASEMODEL_H


#include <vector>
#include "../../Math/Structs.h"

class BaseModel {
private:
protected:
    float
        modelPosXGlobal = .0f,
        modelPosYGlobal = .0f,
        modelPosZGlobal = .0f;


    float
        modelRotXLocal = 0.0f,
        modelRotYLocal = 0.0f,
        modelRotZLocal = 0.0f;
    float
        modelRotKX,
        modelRotKY,
        modelRotKZ,
        modelDegree;
    float
        modelRotXGlobal = 0.0f,
        modelRotYGlobal = 0.0f,
        modelRotZGlobal = 0.0f;
    float
        modelScaleXLocal = 1.0f,
        modelScaleYLocal = 1.0f,
        modelScaleZLocal = 1.0f;

    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;
    Vector3D  tempLookAt{};

    void apudater();
public:
    [[maybe_unused]] virtual void setGlobalPosition(std::vector<float> pos);
    [[maybe_unused]] virtual void setGlobalPosition(Vector3D  pos);
    [[maybe_unused]] virtual void setGlobalPosition(float x,float y,float z);
    [[maybe_unused]] virtual void setGlobalPositionX(float x);
    [[maybe_unused]] virtual void setGlobalPositionY(float y);
    [[maybe_unused]] virtual void setGlobalPositionZ(float z);


    [[maybe_unused]] virtual void setLocalRotation(std::vector<float> rotation);
    [[maybe_unused]] virtual void setLocalRotation(float x,float y,float z);
    [[maybe_unused]] virtual void setLocalRotationX(float x);
    [[maybe_unused]] virtual void setLocalRotationY(float y);
    [[maybe_unused]] virtual void setLocalRotationZ(float z);
    [[maybe_unused]] virtual void setLocalRotationRad(std::vector<float> rotation);
    [[maybe_unused]] virtual void setLocalRotationRad(float x,float y,float z);


    [[maybe_unused]] virtual void setGlobalRotation(std::vector<float> rotation);
    [[maybe_unused]] virtual void setGlobalRotation(float x,float y,float z);
    [[maybe_unused]] virtual void setGlobalRotationX(float x);
    [[maybe_unused]] virtual void setGlobalRotationY(float y);
    [[maybe_unused]] virtual void setGlobalRotationZ(float z);
    [[maybe_unused]] virtual void setGlobalRotationRad(std::vector<float> rotation);
    [[maybe_unused]] virtual void setGlobalRotationRad(float x,float y,float z);


    [[maybe_unused]] virtual void lookAt(std::vector<float> pos);
    [[maybe_unused]] virtual void lookAt(float x,float y,float z);
    [[maybe_unused]] virtual void lookAt(Vector3D pos);


    [[maybe_unused]] virtual void setLocalScale(std::vector<float> pos);
    [[maybe_unused]] virtual void setLocalScale(float x,float y,float z);

    [[maybe_unused]] [[maybe_unused]] virtual void setLocalScaleX(float x);
    [[maybe_unused]] virtual void setLocalScaleY(float y);
    [[maybe_unused]] virtual void setLocalScaleZ(float z);


    [[maybe_unused]] virtual void render(){};

    [[maybe_unused]] virtual void setColor(std::vector<float> color);
    [[maybe_unused]] virtual void setColor(float r,float g,float b,float a);

    [[maybe_unused]] virtual void destroy(){};
};


#endif //MIRAAI_BASEMODEL_H
