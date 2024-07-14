    //
// Created by asuka on 28.05.2024.
//

#ifndef MIRAI_SKELETON_H
#define MIRAI_SKELETON_H

#pragma once
#include "Bone.h"
#include "../Model3D/Model.h"
#include "../../Animations/BoneAnimation.h"
#include "../../Animations/AnimationController.h"

    class Skeleton {
private:
    void resubInit(Model &model);
    Bone mainBone{};
    std::vector<Bone> bonesArray;
    std::vector<std::vector<Bone>> subBonesArray;
    std::vector<std::vector<std::string>> modelsIndexes;
    AnimationController animController;
public:
    Skeleton() = default;
    Skeleton(Model& model);

    void render(PerspectiveCamera camera);
    void render(PerspectiveCamera camera,Model& model);
    void destroy();
};


#endif //MIRAI_SKELETON_H
