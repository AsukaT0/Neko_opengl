//
// Created by asuka on 22.02.2024.
//

#include "FlatModels.h"

#include <utility>
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

FlatModels::FlatModels(std::vector<Vector3D > pointVector) {
    dots = std::move(pointVector);
}

void FlatModels::render() {
    glPushMatrix();
//    std::cout << modelPosXGlobal << " " << modelPosYGlobal << " " << modelPosZGlobal << "\n";
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(modelPosXGlobal, modelPosYGlobal, modelPosZGlobal));

    modelMatrix = glm::rotate(modelMatrix, modelRotXGlobal, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, modelRotYGlobal, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, modelRotZGlobal, glm::vec3(0.0f, 0.0f, 1.0f));
    glMultMatrixf(glm::value_ptr(modelMatrix));

    glBegin(GL_LINE_STRIP);
    for (int ii1 = 1;ii1 < dots.size();ii1++){
        glVertex3f(dots[ii1-1].x, dots[ii1-1].y, dots[ii1-1].z);
        glVertex3f(dots[ii1].x, dots[ii1].y, dots[ii1].z);}
    glEnd();
    for(auto part:modelParts){
        part.render();}
    glPopMatrix();
}

void FlatModels::deleteModelPart(int index) {
    if (index >= 0 && index < modelParts.size()) {
        modelParts.erase(modelParts.begin() + index);}}

FlatModels FlatModels::getModelPart(int index) {
    return modelParts[index];
}

void FlatModels::addModelPart(FlatModels model) {
    modelParts.push_back(model);
}