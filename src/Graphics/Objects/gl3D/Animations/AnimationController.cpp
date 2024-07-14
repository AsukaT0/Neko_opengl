//
// Created by asuka on 01.06.2024.
//

#include "AnimationController.h"

void AnimationController::startAnim(int index) {
    anims[index].start();
}

void AnimationController::render(std::vector<Bone> &bonesArray, Model &model1) {
    for(auto & anim : anims){
        anim.render(bonesArray,model1);}
}

std::vector<BoneAnimation> &AnimationController::getAnimationsList() {
    return anims;
}

void AnimationController::startAll() {
    for(auto & anim : anims){
        anim.start();}
}

void AnimationController::destroy() {
    anims.clear();
}

void AnimationController::addAnim(const std::string& tag, const BoneAnimation &anim) {
    tagger.emplace(tag,anims.size());
    anims.push_back(anim);}
