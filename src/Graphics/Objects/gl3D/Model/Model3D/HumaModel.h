//
// Created by asuka on 29.05.2024.
//

#ifndef MIRAI_HUMAMODEL_H
#define MIRAI_HUMAMODEL_H


#pragma once
#include "Model.h"
#include "../Bones/Skeleton.h"

class HumaModel : public Model{
private:
    std::vector<int> eyeIndex;
    Skeleton skel;
    void processing();
public:
    HumaModel() = default;
    HumaModel(const std::vector<Model::ModelPart>& parts);
    HumaModel(const Model& model);
    void lookAt(const Vertex& pos);
    void render(PerspectiveCamera camera) override;
    void destroy();
};


#endif //MIRAI_HUMAMODEL_H
