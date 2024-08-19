//
// Created by asuka on 01.06.2024.
//

#include "BoneAnimation.h"
#include "../../../Window.h"
#include "../../../../Controller.h"
#include <utility>


BoneAnimation::BoneAnimation(std::function<void(std::vector<Bone> &, Model &, float, float)> function, float time_s,
                             bool cycle, float delay) : function(std::move(function)), cycle(cycle),
                                                        delay2tweenAnim(delay) {
    animTime = time_s;
}

void BoneAnimation::run(std::vector<Bone> &bonesArray, Model &model1) {
    function(bonesArray, model1, 0, 0);
}

double BoneAnimation::calc() {
    return 100.0 / (animTime / Controller::mainWindows->getFramerate());
}

void BoneAnimation::render(std::vector<Bone> &bonesArray, Model &model1) {
    if (status >= 100) {

        if (!cycle) {
            isAlive = false;
            if (end != nullptr) {
                end(bonesArray, model1);}

        } else {

            std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::milliseconds elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    currentTime - startDelay);
            if (end != nullptr) {
                end(bonesArray, model1);}
            if (elapsedTime.count() < delay2tweenAnim) { return; }
            startDelay = currentTime;
        }
        status = 0;
    }
    if (isAlive) {
        status += calc();
        if (function != nullptr)
            function(bonesArray, model1, status, calc());

    }
}

void BoneAnimation::start() {
    isAlive = true;
    status = 0;
    startDelay = std::chrono::high_resolution_clock::now();
}

void BoneAnimation::destroy() {
    isAlive = false;
    function = nullptr;
}

void BoneAnimation::setOnEnd(const std::function<void(std::vector<Bone> &, Model &)> &func) {
    end = func;
}

void BoneAnimation::setDelay(long long delay) {
    delay2tweenAnim = delay;
}

BoneAnimation::BoneAnimation(std::function<void(std::vector<Bone> &, Model &, float, float)> function,
                             std::function<void(std::vector<Bone> &, Model &)> endFunction, float time_s, bool cycle,
                             float delay) :
        function(std::move(function)), end(std::move(endFunction)), cycle(cycle), delay2tweenAnim(delay) {
    animTime = time_s;
}
