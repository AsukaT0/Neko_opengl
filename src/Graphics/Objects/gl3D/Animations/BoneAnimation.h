//
// Created by asuka on 01.06.2024.
//

#ifndef MIRAI_BONEANIMATION_H
#define MIRAI_BONEANIMATION_H

#include "../Model/Model3D/Model.h"
#include "../Model/Bones/Bone.h"
#include <functional>
#include <utility>
#include <chrono>
#include <Utils/SwitcherString.h>

class BoneAnimation {
    std::function<void (std::vector<Bone>& bonesArray,Model &model1,float status,float step)> function = nullptr;
    std::function<void (std::vector<Bone>& bonesArray,Model &model1)> end = nullptr;
    double calc();
    bool isAlive = false,cycle = false;
    float status = 0,animTime=0;
    std::chrono::high_resolution_clock::time_point startDelay;
    long long delay2tweenAnim = 0;
public:

    BoneAnimation() = default;
    BoneAnimation(std::function<void (std::vector<Bone>& bonesArray,Model &model1,float status,float step)> function,
                  float time_s = 1,bool cycle = false,float delay = 1000);
    BoneAnimation(std::function<void (std::vector<Bone>& bonesArray,Model &model1,float status,float step)> function,
                  std::function<void (std::vector<Bone>& bonesArray,Model &model1)> endFunction,
                  float time_s = 1,bool cycle = false,float delay = 1000);
    void start();
    void setOnEnd(const std::function<void(std::vector<Bone> &, Model &)>& func);
    void render(std::vector<Bone>& bonesArray,Model &model1);
    void run(std::vector<Bone>& bonesArray,Model &model1);
    void destroy();
    void setDelay(long long delay);
};


#endif //MIRAI_BONEANIMATION_H
