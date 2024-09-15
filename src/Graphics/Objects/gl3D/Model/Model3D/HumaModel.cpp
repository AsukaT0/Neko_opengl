//
// Created by asuka on 29.05.2024.
//
#include <cmath>
#include <thread>
#include "HumaModel.h"
#include "../../../../Math/Functionality.h"

void HumaModel::processing() {
    for (int i = 0; i < getParts().size(); i++) {
        auto& part = getParts()[i];
        if (part.getTag().starts_with("eye_")) {
            eyeIndex.push_back(i);
        }
    }

}


HumaModel::HumaModel(const std::vector<Model::ModelPart>& parts) : Model(parts) {
    processing();
}

HumaModel::HumaModel(const Model& model) : Model(model) {
    processing();
}

enum AxisType { Left, Right };

double eyeBarrierX(double angleX, AxisType type) {
    return std::clamp(angleX, -45.0, 45.0)/425.0f;
}

double eyeBarrierY(double angleY, AxisType type) {
    if (angleY < -180) {
        angleY += 360;}
    return std::clamp(angleY, (type == AxisType::Left) ? -70.0 : -20.0, (type == AxisType::Left) ? 20.0 : 70.0)/425.0f;
}
constexpr float D2R = (180 / M_PI);
void HumaModel::lookAt(const Vertex& pos) {
    for (int index : eyeIndex) {
        auto& part = getParts()[index];
        AxisType axisType = (part.getGeomCenter().x < 0) ? AxisType::Left : AxisType::Right;

        float distance1 = Functionality::dist2tweenP(pos, part.getGeomCenter());
        Vertex distVertex = part.getGeomCenter() - pos;
        float angleX = 90 - acosf(distVertex.y / distance1) * D2R;
        float angleY = -atan2f(distVertex.z, distVertex.x) * D2R - 90;
        angleY = eyeBarrierY(angleY, axisType);
        angleX = eyeBarrierX(angleX, axisType);
        part.getTextureCoords().setBias(angleY, angleX);
    }
}

void HumaModel::render(PerspectiveCamera camera) {
    Model::render(camera);
    skel.render(camera);
}

void HumaModel::destroy() {
    skel.destroy();
    Model::destroy();
}

void HumaModel::loadSkeleton(const std::string& path) {
    std::thread([this, path]{
        skel = Skeleton();
        skel.loadSkelFile(path);
        skel.initSkeleton(this);
    }).join();
}
