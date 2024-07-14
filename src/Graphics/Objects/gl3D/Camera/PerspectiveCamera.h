//
// Created by asuka on 21.02.2024.
//

#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

#ifndef MIRAAI_PERSPECTIVECAMERA_H
#define MIRAAI_PERSPECTIVECAMERA_H

class PerspectiveCamera {
private:
    float posCamX = 0,posCamY = 0,posCamZ = 0;
    float posLookX = 0,posLookY = 0,posLookZ = 0;
    float upX = 0,upY = 1,upZ = 0;
    float angle = 60;
    float aspect = 1;
    float near = 0.1;
    float far = 100;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
public:
    PerspectiveCamera(float viewingAngle,float aspect,float near,float far,float posX,float posY,float posZ,float lookX,float lookY,float lookZ,float upX,float upY,float upZ);
    PerspectiveCamera(float viewingAngle,float aspect,float near,float far,std::vector<float> pos,std::vector<float> look,std::vector<float> up);



    [[maybe_unused]] void setPosition(float posX,float posY,float posZ);
    [[maybe_unused]] void setPosition(std::vector<float> pos);
    [[maybe_unused]] void setLookAt(float lookX,float lookY,float lookZ);
    [[maybe_unused]] void setLookAt(std::vector<float> look);

    [[maybe_unused]] void setUp(float upX,float upY,float upZ);

    [[maybe_unused]] void setUp(std::vector<float> up);
    void render();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    PerspectiveCamera();
    glm::vec3 getPosition();
};


#endif //MIRAAI_PERSPECTIVECAMERA_H
