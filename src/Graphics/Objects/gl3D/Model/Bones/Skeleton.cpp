//
// Created by asuka on 28.05.2024.
//
#include <csignal>
#include <thread>
#include <cmath>
#include <unordered_set>
#include <File/File.h>
#include <algorithm>
#include "Skeleton.h"
#include "../../../../Window.h"
#include "../../../../Math/Functionality.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


void skinWModel(Model &part, const std::string &tag, std::vector<Bone> &bones, unsigned int numActiveBones = 6) {
    vertarr &vert = part.getModelPartByTag(tag).getVertex();
    int vertSize = (int) vert.size();
    size_t bonesSize = bones.size();
    for (int ii1 = 0; ii1 < vertSize; ++ii1) {
        std::vector<std::pair<float, int>> weights;
        for (size_t ii2 = 0; ii2 < bonesSize; ++ii2) {
            weights.emplace_back(Functionality::distToSeg(vert[ii1], bones[ii2].getStart(), bones[ii2].getEnd()), ii2);
        }

        std::sort(weights.begin(), weights.end());
        float sum = 0;
        for (size_t ii2 = 0; ii2 < std::min((unsigned int) weights.size(), numActiveBones); ++ii2) {
            sum += powf(1 / weights[ii2].first, (float) numActiveBones);
        }
        for (int ii2 = 0; ii2 < std::min((unsigned int) weights.size(), numActiveBones); ++ii2) {
            bones[weights[ii2].second].addIndices(tag, ii1, powf(1 / weights[ii2].first, (float) numActiveBones) / sum);
        }
    }
}


void clap(std::vector<Bone> &bone, std::vector<Bone> &destBone, int &b) {
    for (auto &ii1: destBone) {
        ii1.shift(b);
    }
    b += (int) destBone.size();
    for (auto &m: destBone) { bone.push_back(m); }
    destBone.clear();
}

void Skeleton::render(PerspectiveCamera camera) {
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(glm::value_ptr(camera.getProjectionMatrix() * camera.getViewMatrix()));
//    glEnable(GL_DEPTH);
    animController.render(bonesArray, localModel);
//    for (Bone &b: bonesArray) {b.render();}
//    glDisable(GL_DEPTH);
    glPopMatrix();
}

void Skeleton::destroy() {
    animController.destroy();
    bonesArray.clear();
}

void Skeleton::initSkeleton(Model *model) {
    localModel = model;
    resubInit();
    initAnimations();
}


void Skeleton::resubInit() {
    int b = 0;
    for (int ii1 = 0; ii1 < subBonesArray.size(); ii1++) {
        for (auto &bone: subBonesArray[ii1]) {
            bone.setModelPart(localModel);}
        for (int ii2 = 0; ii2 < modelsIndexes[ii1].size(); ii2++)
            skinWModel(*localModel, modelsIndexes[ii1][ii2], subBonesArray[ii1]);
        clap(bonesArray, subBonesArray[ii1], b);
    }
}

void Skeleton::initAnimations() {
    animController.addAnim("tail_anim_001", {
            [](std::vector<Bone> &bones, Model *model1, float status, float step) {
                auto f = [](float start, float iter, float step) {
                    return glm::radians(sinf(glm::radians(start * 3.6 - 11.0 * iter)) / 40.0);
                };
                bones[28].funRAPZ(bones[28].getStart(), 0, f, status, 0);
                bones[28].funRAPY(bones[28].getStart(), 0, f, status, 0);
            },
            [](std::vector<Bone> &bones, Model *model1) {
                bones[28].reset();
            }, 2, true, 0});

    animController.addAnim("blink_anim_001", {
            [](std::vector<Bone> &bones, Model *model1, float status,
               float step) {
                float prestep = sinf(M_PI * (status / 100));
                bones[30].setRotationX(bones[30].getEnd(), (M_PI / 3.3) * prestep);
                bones[30].setRotationZ(bones[30].getEnd(), (-M_PI / 6.5) * prestep);
                bones[36].setRotationX(bones[36].getEnd(), (M_PI / 3.3) * prestep);
                bones[36].setRotationZ(bones[36].getEnd(), (M_PI / 6.5) * prestep);
            },
            [](std::vector<Bone> &bones, Model *model1) {
                model1->getModelPartByTag("eye_above_r.001").reset();
                model1->getModelPartByTag("eye_above_l.002").reset();
            },
            0.3, true, 4000});
    animController.addAnim("ear_anim_001", {
            [](std::vector<Bone> &bones, Model *model1, float status, float step) {
                float prestep = sinf(M_PI * (status / 100));

                bones[42].setRotationX((M_PI / 8) * prestep);
                bones[42].setRotationZ((M_PI / 16) * prestep);
                bones[44].setRotationX((M_PI / 8) * prestep);
                bones[44].setRotationZ(-(M_PI / 16) * prestep);
            },
            [](std::vector<Bone> &bones, Model *model1) {
                bones[42].reset();
                bones[44].reset();
            }, 0.1, true, 5000});
    animController.addAnim("left_knee_anim_001", {
            [](std::vector<Bone> &bones, Model *model1, float status, float step) {
                bones[56 + 2].rotAPY(bones[56 + 2].getStart(),
                                     (status > 50) ? M_PIf * (step / 100) : -M_PIf * (step / 100));
            },
            [](std::vector<Bone> &bones, Model *model1) {
                bones[56 + 2].reset();
            },
            2, true, 0});
    animController.startAll();
}

void Skeleton::loadSkelFile(std::string path) {
    boost::property_tree::ptree pt;
    try { boost::property_tree::read_json(path, pt); }
    catch (const std::exception &e) {
        std::cerr << "Ошибка чтения JSON: " << e.what() << std::endl;
        return;
    }
    for (const auto &k: pt.get_child("bones")) {
        modelsIndexes.emplace_back();
        unsigned char lastOfModelsIndexes = modelsIndexes.size() - 1;
        for (const auto &s: k.second.get_child("id")) {
            modelsIndexes[lastOfModelsIndexes].push_back(s.second.get<std::string>(""));}
        subBonesArray.emplace_back();
        unsigned char lastOfSubBonesArray = subBonesArray.size() - 1;
        for (const auto &i: k.second.get_child("bones")) {
            subBonesArray[lastOfSubBonesArray].emplace_back();
            unsigned char lastOfLastSubBonesArray = subBonesArray[lastOfSubBonesArray].size() - 1;
            subBonesArray[lastOfSubBonesArray][lastOfLastSubBonesArray]
                    .setPos(
                            {i.second.get<float>("start.x"),
                             i.second.get<float>("start.y"),
                             i.second.get<float>("start.z")},
                            {i.second.get<float>("end.x"),
                             i.second.get<float>("end.y"),
                             i.second.get<float>("end.z")});
            subBonesArray[lastOfSubBonesArray][lastOfLastSubBonesArray]
                    .setModelIDs(modelsIndexes[lastOfModelsIndexes]);
            subBonesArray[lastOfSubBonesArray][lastOfLastSubBonesArray]
                    .setBonesArray(&bonesArray);
        }
        for (int ii1 = 0; ii1 < subBonesArray[lastOfSubBonesArray].size(); ii1++) {
            for (int ii2 = 0; ii2 < subBonesArray[lastOfSubBonesArray].size(); ii2++) {
                if (subBonesArray[lastOfSubBonesArray][ii1].getStart() ==
                    subBonesArray[lastOfSubBonesArray][ii2].getEnd()) {
                    subBonesArray[lastOfSubBonesArray][ii2].addChild(ii1);
                    subBonesArray[lastOfSubBonesArray][ii1].setParent(ii2);
                }
            }
        }
    }
}
