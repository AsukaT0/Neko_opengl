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



void Skeleton::render(PerspectiveCamera camera) {
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(glm::value_ptr(camera.getProjectionMatrix() * camera.getViewMatrix()));
    glEnable(GL_DEPTH);
    for (Bone b: bonesArray) {
        b.render();
    }
    glDisable(GL_DEPTH);
    glPopMatrix();
}

void skinWModel(Model &part, const std::string &tag, std::vector<Bone> &bones, int numActiveBones = 5) {
    vertarr &vert = part.getModelPartByTag(tag).getVertex();
    int vertSize = (int) vert.size();
    size_t bonesSize = bones.size();
    for (int ii1 = 0; ii1 < vertSize; ++ii1) {
        std::vector<std::pair<float, int>> weights;
        for (size_t ii2 = 0; ii2 < bonesSize; ++ii2) {
            weights.emplace_back(Functionality::distToSeg(vert[ii1], bones[ii2].getStart(), bones[ii2].getEnd()), ii2);
        }
        std::sort(weights.begin(), weights.end());
        float ammm = 0;
        int count = 0;
        for (size_t ii2 = 0; ii2 < std::min((int) weights.size(), numActiveBones); ++ii2) {
            ammm += powf(1 / weights[ii2].first, 3);
            count += 1;
        }
        for (int ii2 = 0; ii2 < count; ++ii2) {
            bones[weights[ii2].second].addIndices(tag, ii1, powf(1 / weights[ii2].first, 3) / ammm);
        }
    }
}

void subInit(Model &model, std::vector<Bone> &bones, const std::vector<std::string> &modelIndex) {
    for (int ii1 = 0; ii1 < bones.size(); ii1++) {
        bones[ii1].setModelPart(model);
        bones[ii1].setModelIDs(modelIndex);
        for (int ii2 = 0; ii2 < bones.size(); ii2++) {
            if (bones[ii1].getStart() == bones[ii2].getEnd()) {
                bones[ii2].addChild(ii1);
                bones[ii1].setParent(ii2);
            }
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

void Skeleton::render(PerspectiveCamera camera, Model &model) {
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(glm::value_ptr(camera.getProjectionMatrix() * camera.getViewMatrix()));
    glEnable(GL_DEPTH);
    animController.render(bonesArray, model);
//    for (Bone &b: bonesArray) {b.render();}
    glDisable(GL_DEPTH);
    glPopMatrix();
}

void Skeleton::destroy() {
    animController.destroy();
    bonesArray.clear();
}

void Skeleton::initSkeleton(Model &model) {
    resubInit(model);
    initAnimations();
}


void Skeleton::resubInit(Model &model) {
    std::vector<std::string> tags;
    for (auto &ii1: model.getParts()) {
        std::string tete = ii1.getTag();
        tags.push_back(tete);
        if (tete.starts_with("tail_")) {
            modelsIndexes[0].push_back(tete);
        } else if (tete.starts_with("eye_above_r")) {
            modelsIndexes[1].push_back(tete);
        } else if (tete.starts_with("eye_above_l")) {
            modelsIndexes[2].push_back(tete);
        } else if (tete.starts_with("ear_r")) {
            modelsIndexes[3].push_back(tete);
        } else if (tete.starts_with("ear_l")) {
            modelsIndexes[4].push_back(tete);
        } else if (
                tete.starts_with("body_") ||
                tete.starts_with("sock_l") ||
                tete.starts_with("sock_r") ||
                tete.starts_with("pantsu_") ||
                tete.starts_with("fragmemt_") ||
                tete.starts_with("fragmemt_") ||
                tete.starts_with("chocker_") ||
                tete.starts_with("clother_")
                ) {
            modelsIndexes[5].push_back(tete);
        }
    }
    int b = 0;
    for (int ii1 = 0; ii1 < subBonesArray.size(); ii1++) {
        subInit(model, subBonesArray[ii1], modelsIndexes[ii1]);
        for (int ii2 = 0; ii2 < modelsIndexes[ii1].size(); ii2++)
            skinWModel(model, modelsIndexes[ii1][ii2], subBonesArray[ii1]);
        clap(bonesArray, subBonesArray[ii1], b);
    }
}

void Skeleton::saveSkeleton() {
    ConfigLoader config{};
    std::vector<std::variant<int, float, bool, std::string, ConfigLoader>> data;
    for (auto &k: bonesArray) {
        std::vector<std::variant<int, float, bool, std::string, ConfigLoader>> configVector;
        std::vector<std::variant<int, float, bool, std::string, ConfigLoader>> childsVector;
        for (auto i: k.getData()) {
            std::vector<std::variant<int, float, bool, std::string, ConfigLoader>> vector0;
            std::vector<std::variant<int, float, bool, std::string, ConfigLoader>> vector1;
            for (auto j: i.weightAndIndexArray) {
                vector0.emplace_back(j.i);
                vector1.emplace_back(j.w);
            }
            ConfigLoader subConfig2;
            subConfig2.set("model_id", i.modelID);
            subConfig2.set("vertex_indexes", vector0);
            subConfig2.set("vertex_weights", vector1);
            configVector.emplace_back(subConfig2);
        }
        for (auto i: k.getChilds()) {
            childsVector.emplace_back(i);
        }

        ConfigLoader subConfig0;
        subConfig0.set("start_bone",
                       std::vector<std::variant<int, float, bool, std::string, ConfigLoader>>{k.getStart().x,
                                                                                              k.getStart().y,
                                                                                              k.getStart().z});
        subConfig0.set("end_bone", std::vector<std::variant<int, float, bool, std::string, ConfigLoader>>{k.getEnd().x,
                                                                                                          k.getEnd().y,
                                                                                                          k.getEnd().z});
        subConfig0.set("data", configVector);
        subConfig0.set("parent", k.getParent());
        subConfig0.set("childs", childsVector);
        data.emplace_back(subConfig0);
    }
//    config.set("bone_data", data);
//    config.saveToPath("skel.mds");
}

void Skeleton::initAnimations() {
    animController.addAnim("tail_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status, float step) {
                auto f = [](float start, float iter, float step) {
                    return glm::radians(sinf(glm::radians(start * 3.6 - 11.0 * iter)) / 40.0);
                };
                bones[28].funRAPZ(model1, bones, bones[28].getStart(), 0, f, status, 0);
                bones[28].funRAPY(model1, bones, bones[28].getStart(), 0, f, status, 0);
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[28].reset(model1, bones);
            }, 2, true, 0});

    animController.addAnim("blink_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status,
               float step) {
                float prestep = sinf(M_PI * (status / 100));
                bones[30].setRotationX(model1, bones, bones[30].getEnd(), (M_PI / 3.3) * prestep);
                bones[30].setRotationZ(model1, bones, bones[30].getEnd(), (-M_PI / 6.5) * prestep);
                bones[36].setRotationX(model1, bones, bones[36].getEnd(), (M_PI / 3.3) * prestep);
                bones[36].setRotationZ(model1, bones, bones[36].getEnd(), (M_PI / 6.5) * prestep);
            },
            [](std::vector<Bone> &bones, Model &model1) {
                model1.getModelPartByTag("eye_above_r.001").reset();
                model1.getModelPartByTag("eye_above_l.002").reset();
            },
            0.3, true, 4000});
    animController.addAnim("ear_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status, float step) {
                float prestep = sinf(M_PI * (status / 100));

                bones[42].setRotationX(model1, bones, (M_PI / 8) * prestep);
                bones[42].setRotationZ(model1, bones, (M_PI / 16) * prestep);
                bones[44].setRotationX(model1, bones, (M_PI / 8) * prestep);
                bones[44].setRotationZ(model1, bones, -(M_PI / 16) * prestep);
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[42].reset(model1, bones);
                bones[44].reset(model1, bones);
            }, 0.1, true, 5000});
    animController.addAnim("left_knee_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status, float step) {
                bones[56 + 3].rotAPY(model1, bones, bones[56 + 2].getStart(),
                                     (status > 50) ? M_PIf * (step / 100) : -M_PIf * (step / 100));
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[56 + 3].reset(model1, bones);
            },
            2, true, 0});
    animController.startAll();
}

void Skeleton::loadSkelFile(std::string path) {
    boost::property_tree::ptree pt;
    try { boost::property_tree::read_json(path, pt); }
    catch (const std::exception &e) {
        std::cerr << "Ошибка чтения JSON: " << e.what() << std::endl;
        return;}

    for (const auto& k: pt) {
        subBonesArray.emplace_back();
        modelsIndexes.emplace_back();
        for (const auto& i: k.second) {
            subBonesArray[subBonesArray.size()-1].emplace_back();
            subBonesArray[subBonesArray.size()-1][subBonesArray[subBonesArray.size()-1].size()-1].setPos(
                    {i.second.get<float>("start.x"),
                        i.second.get<float>("start.y"),
                        i.second.get<float>("start.z")},
                    {i.second.get<float>("end.x"),
                        i.second.get<float>("end.y"),
                        i.second.get<float>("end.z")});
        }
    }
//    rawArray = firstLayerVector;
}
