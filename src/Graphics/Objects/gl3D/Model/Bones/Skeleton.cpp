//
// Created by asuka on 28.05.2024.
//
#include <csignal>
#include <thread>
#include <cmath>
#include <unordered_set>
#include "Skeleton.h"
#include "../../../../Window.h"
#include "../../../../Math/Functionality.h"

Skeleton::Skeleton(Model &model) {
    animController.addAnim("tail_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status, float step) {
                auto f = [](float start, float iter, float step) {
                    return glm::radians(sinf(glm::radians(start * 3.6 - 11.0 * iter)) / 40.0);};
                bones[28].funRAPZ(model1, bones, bones[28].getStart(), 0, f, status, 0);
                bones[28].funRAPY(model1, bones, bones[28].getStart(), 0, f, status, 0);
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[28].reset(model1, bones);
            }, 2, true, 0});
    animController.addAnim("blink_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status,
               float step) {
                float percent_step = (step / 100);
                bones[30].rotAPX(model1, bones, bones[30].getEnd(),
                                 (status > 50) ? -3.0f * percent_step :  3.0f * percent_step);
                bones[30].rotAPZ(model1, bones, bones[30].getEnd(),
                                 (status > 50) ?  1.4f * percent_step : -1.4f * percent_step);
                bones[36].rotAPX(model1, bones, bones[36].getEnd(),
                                 (status > 50) ? -3.0f * percent_step :  3.0f * percent_step);
                bones[36].rotAPZ(model1, bones, bones[36].getEnd(),
                                 (status > 50) ? -1.4f * percent_step :  1.4f * percent_step);
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[30].reset(model1, bones);
                bones[36].reset(model1, bones);
            },
            0.3, true, 4000});

    animController.addAnim("ear_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status, float step) {
                float percent_step = (step / 100);
                bones[42].rotX(model1, bones, (status > 50) ? -0.5f * percent_step :  0.5f * percent_step);
                bones[42].rotZ(model1, bones, (status > 50) ?  0.2f * percent_step : -0.2f * percent_step);
                bones[44].rotX(model1, bones, (status > 50) ? -0.5f * percent_step :  0.5f * percent_step);
                bones[44].rotZ(model1, bones, (status > 50) ? -0.2f * percent_step :  0.2f * percent_step);
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[42].reset(model1, bones);
                bones[44].reset(model1, bones);
            }, 0.1, true, 5000});
    animController.addAnim("left_knee_anim_001", {
            [](std::vector<Bone> &bones, Model &model1, float status, float step) {
                bones[56 + 3].rotAPY(model1, bones,bones[56 + 2].getStart(), (status > 50) ? M_PIf * (step / 100) : -M_PIf * (step / 100));
            },
            [](std::vector<Bone> &bones, Model &model1) {
                bones[56 + 3].reset(model1, bones);
            },
            2, true, 0});
    animController.startAll();
    resubInit(model);
}

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

void skinWModel(Model &part, const std::string &tag, std::vector<Bone> &bones, int numActiveBones = 3) {
    vertarr &vert = part.getModelPartByTag(tag).getVertex();
    int vertSize = (int)vert.size();
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

void subInit(vertarr biases, Model &model, std::vector<Bone> &bones, const std::vector<std::string> &modelIndex) {
    for (int ii1 = 0; ii1 < biases.size() - 1; ii1 += 2) {
        bones.emplace_back(model, modelIndex);
        bones[bones.size() - 1].setPos(biases[ii1], biases[ii1 + 1]);
    }
    for (int ii1 = 0; ii1 < bones.size(); ii1++) {
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
    b += (int)destBone.size();
    for (auto &m: destBone) { bone.push_back(m); }
    destBone.clear();
}

void Skeleton::render(PerspectiveCamera camera, Model &model) {
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(glm::value_ptr(camera.getProjectionMatrix() * camera.getViewMatrix()));
    glEnable(GL_DEPTH);
    animController.render(bonesArray, model);
    for (Bone &b: bonesArray) {
//        b.render();
    }
    glDisable(GL_DEPTH);
    glPopMatrix();
}

void Skeleton::destroy() {
    animController.destroy();
    bonesArray.clear();
}


void Skeleton::resubInit(Model &model) {
    std::vector<std::string> tags;
    std::vector<vertarr > biases = {{
                                            {-0.053564, 7.08008, -4.80352},
                                            {-0.049707, 7.17831, -5.19524},

                                            {-0.062266, 6.87031, -4.40684},
                                            {-0.053564, 7.08008, -4.80352},

                                            {-0.061695, 6.55297, -4.0781},
                                            {-0.062266, 6.87031, -4.40684},

                                            {-0.04887,  6.155,   -3.83981},
                                            {-0.061695, 6.55297, -4.0781},

                                            {-0.035553, 5.70906, -3.67459},
                                            {-0.04887,  6.155,   -3.83981},

                                            {-0.026861, 5.23557, -3.56633},
                                            {-0.035553, 5.70906, -3.67459},

                                            {-0.01808, 4.74519, -3.51419},
                                            {-0.026861, 5.23557,  -3.56633},

                                            {-0.002169, 4.24626,  -3.52945},
                                            {-0.01808, 4.74519,  -3.51419},

                                            {0.019576, 3.74898, -3.6181},
                                            {-0.002169, 4.24626, -3.52945},

                                            {0.041103,  3.25977, -3.75447},
                                            {0.019576, 3.74898,  -3.6181},

                                            {0.05913,  2.77841,  -3.90687},
                                            {0.041103, 3.25977,  -3.75447},

                                            {0.069297, 2.3023, -4.05239},
                                            {0.05913, 2.77841, -3.90687},

                                            {0.067469, 1.82898, -4.16931},
                                            {0.069297, 2.3023,  -4.05239},

                                            {0.051672, 1.3596,  -4.23998},
                                            {0.067469, 1.82898, -4.16931},

                                            {0.023121, 0.898221, -4.25099},
                                            {0.051672, 1.3596,  -4.23998},

                                            {-0.012288, 0.453202, -4.19599},
                                            {0.023121, 0.898221, -4.25099},

                                            {-0.046421, 0.039268, -4.07447},
                                            {-0.012288, 0.453202, -4.19599},

                                            {-0.072928, -0.327027, -3.89143},
                                            {-0.046421, 0.039268, -4.07447},

                                            {-0.087724, -0.62109, -3.64804},
                                            {-0.072928, -0.327027, -3.89143},

                                            {-0.089876, -0.818076, -3.35023},
                                            {-0.087724, -0.62109, -3.64804},

                                            {-0.08268, -0.909413, -3.0167},
                                            {-0.089876, -0.818076, -3.35023},

                                            {-0.070752, -0.892121, -2.66808},
                                            {-0.08268, -0.909413, -3.0167},

                                            {-0.059165, -0.763522, -2.32853},
                                            {-0.070752, -0.892121, -2.66808},

                                            {-0.052303, -0.533468, -2.02419},
                                            {-0.059165, -0.763522, -2.32853},

                                            {-0.052191, -0.219504, -1.77547},
                                            {-0.052303, -0.533468, -2.02419},

                                            {-0.059254, 0.170634, -1.60406},
                                            {-0.052191, -0.219504, -1.77547},

                                            {-0.071675, 0.63289, -1.5323},
                                            {-0.059254, 0.170634, -1.60406},

                                            {-0.083685, 1.18661, -1.59743},
                                            {-0.071675, 0.63289, -1.5323},

                                            {-0.07908, 1.84841, -1.51981},
                                            {-0.083685, 1.18661, -1.59743},

                                            {-0.029969, 2.36406, -1.06064},
                                            {-0.07908, 1.84841, -1.51981},
                                    },
                                    {
                                            {0.416586,  7.98582, 0.202355},
                                            {0.416586,  7.98582, 0.302355},

                                            {0.416586,  7.98582, 0.302355},
                                            {0.296089,  8.02298, 0.67377},

                                            {0.416586,  7.98582, 0.302355},
                                            {0.395427,  8.09622, 0.667581},

                                            {0.416586,  7.98582, 0.302355},
                                            {0.635996,  8.12945, 0.573539},

                                            {0.416586,  7.98582, 0.302355},
                                            {0.758983,  8.03685, 0.445559},

                                            {0.416586,  7.98582, 0.302355},
                                            {0.802937,  7.99244, 0.339497},
                                    },
                                    {
                                            {-0.416586, 7.98582, 0.202355},
                                            {-0.416586, 7.98582, 0.302355},

                                            {-0.416586, 7.98582, 0.302355},
                                            {-0.296089, 8.02298, 0.67377},

                                            {-0.416586, 7.98582, 0.302355},
                                            {-0.395427, 8.09622, 0.667581},

                                            {-0.416586, 7.98582, 0.302355},
                                            {-0.635996, 8.12945, 0.573539},

                                            {-0.416586, 7.98582, 0.302355},
                                            {-0.758983, 8.03685, 0.445559},

                                            {-0.416586, 7.98582, 0.302355},
                                            {-0.802937, 7.99244, 0.339497},
                                    },
                                    {
                                            {0.163675,  8.36924, -0.240251},
                                            {0.665174,  9.19779, 0.015207},

                                            {0.665174,  9.19779, 0.015207},
                                            {1.00212,   9.6064,  0.063697},
                                    },
                                    {
                                            {-0.03675,  8.36924, -0.240251},
                                            {-0.716714, 9.14159, 0.064511},

                                            {-0.716714, 9.14159, 0.064511},
                                            {-0.984926, 9.58033, 0.256566},
                                    },
                                    {
                                            //main
                                            {0,         1.81564, -0.13825},//4
                                            {0,         3.88377, -0.216172},//4

                                            {0,         3.88377, -0.216172},//0
                                            {0,         5.9519,  -0.294095},//0

                                            {0,         5.9519,  -0.294095},//1
                                            {0,         6.48464, -0.249493},//1

                                            {0,         6.48464, -0.249493},//2
                                            {0,         7.16352, -0.156619},//2

                                            {0,         7.16352, -0.156619},//3
                                            {0,         8.36924, -0.240251},//3

                                            //Left leg
                                            {0,         1.81564, -0.13825},//5
                                            {0.983355,  1.04536, -0.316117},//5

                                            {0.983355, 1.04536, -0.316117},//6
                                            {0.98945,   -3.53381, -0.220758},//6

                                            {0.98945,   -3.53381, -0.220758},//7
                                            {0.98945,  -8.53381, -1.420758},//7
                                            //Right leg
                                            {-0,       1.81564, -0.13825},//8
                                            {-0.983355, 1.04536, -0.316117},//8

                                            {-0.983355, 1.04536, -0.316117},//9
                                            {-0.98945, -3.53381, -0.220758},//9

                                            {-0.98945, -3.53381, -0.220758},//10
                                            {-0.98945, -8.53381, -1.420758},//10
                                            //Left arm
                                            {0,        5.9519, -0.294095},//11
                                            {1.13268, 5.8519,  -0.294095},//11

                                            {1.13268,  5.8519,  -0.294095},//12
                                            {1.14268,  5.8519,  -0.294095},//12

                                            {1.14268,  5.8519,  -0.294095},//12
                                            {1.15268,  5.8519,  -0.294095},//12

                                            {1.15268,  5.8519,  -0.294095},//12
                                            {4.36115,  5.70152, -0.33753},//12

                                            {4.36115,  5.70152, -0.33753},//13
                                            {6.37529,  5.77416,  -0.172668},//13
                                            //finger no1
                                            {6.37529,  5.77416, -0.172668},//14
                                            {6.94184,   5.59265,  0.251696},//14

                                            {6.94184,  5.59265,  0.251696},//15
                                            {7.06873,   5.53094,  0.33554},//15

                                            {7.06873,   5.53094,  0.33554},//16
                                            {7.4888,    5.41798,   0.486827},//16
                                            //finger no2-5
                                            {6.37529,   5.77416,  -0.172668},//17
                                            {6.73059,   5.73378,  -0.088889},//17
                                            //finger no2
                                            {6.73059,   5.73378,   -0.088889},//18
                                            {7.2377,    5.77462,   0.188738},//18

                                            {7.2377,    5.77462,  0.188738},//19
                                            {7.64508,  5.76957,   0.245342},//19

                                            {7.64508,   5.76957,   0.245342},//20
                                            {7.94883,   5.80196,   0.25603},//20
                                            //finger no3
                                            {6.73059,  5.73378,   -0.088889},//21
                                            {7.28985,   5.80024,   -0.017824},//21

                                            {7.28985,   5.80024,   -0.017824},//22
                                            {7.68179,   5.76658,   0.027043},//22

                                            {7.68179,   5.76658,   0.027043},//23
                                            {8.04288,   5.79896,   0.037768},//23
                                            //finger no4
                                            {6.73059,   5.73378,   -0.088889},//24
                                            {7.24217,   5.72373,  -0.218898},//24

                                            {7.24217,   5.72373,   -0.218898},//25
                                            {7.64346,   5.73331, -0.181368},//25

                                            {7.64346,   5.73331,  -0.181368},//26
                                            {7.98145,   5.75932, -0.170287},//26
                                            //finger no5
                                            {6.73059,   5.73378, -0.088889},//27
                                            {7.22236,  5.71026, -0.382476},//27

                                            {7.22236,   5.71026, -0.382476},//28
                                            {7.51266,   5.67161, -0.345191},//28

                                            {7.51266,  5.67161, -0.345191},//29
                                            {7.7836, 5.69357, -0.33855},//29
                                            //Right arm
                                            {-0, 5.9519, -0.294095},//30
                                            {-1.43268, 5.8519, -0.294095},//30

                                            {-1.43268, 5.8519, -0.294095},//31
                                            {-4.36115, 5.70152, -0.33753},//31

                                            {-4.36115, 5.70152, -0.33753},//32
                                            {-6.47529, 5.77416, -0.172668},//32
                                            //finger no1
                                            {-6.47529, 5.77416, -0.172668},//33
                                            {-6.94184, 5.59265, 0.251696},//33

                                            {-6.94184, 5.59265, 0.251696},//34
                                            {-7.06873, 5.53094, 0.33554},//34

                                            {-7.06873, 5.53094, 0.33554},//35
                                            {-7.4888, 5.41798, 0.486827},//35
                                            //finger no2-5
                                            {-6.47529, 5.77416, -0.172668},//36
                                            {-6.73059, 5.73378, -0.088889},//36
                                            //finger no2
                                            {-6.73059, 5.73378, -0.088889},//37
                                            {-7.2377, 5.77462, 0.188738},//37

                                            {-7.2377, 5.77462, 0.188738},//38
                                            {-7.64508, 5.76957, 0.245342},//38

                                            {-7.64508, 5.76957, 0.245342},//39
                                            {-7.94883, 5.80196, 0.25603},//39
                                            //finger no3
                                            {-6.73059, 5.73378, -0.088889},//40
                                            {-7.28985, 5.80024, -0.017824},//40

                                            {-7.28985, 5.80024, -0.017824},//41
                                            {-7.68179, 5.76658, 0.027043},//41

                                            {-7.68179, 5.76658, 0.027043},//42
                                            {-8.04288, 5.79896, 0.037768},//42
                                            //finger no4
                                            {-6.73059, 5.73378, -0.088889},//43
                                            {-7.24217, 5.72373, -0.218898},//43

                                            {-7.24217, 5.72373, -0.218898},//44
                                            {-7.64346, 5.73331, -0.181368},//44

                                            {-7.64346, 5.73331, -0.181368},//44
                                            {-7.98145, 5.75932, -0.170287},//44
                                            //finger no5
                                            {-6.73059, 5.73378, -0.088889},//45
                                            {-7.22236, 5.71026, -0.382476},//45

                                            {-7.22236, 5.71026, -0.382476},//46
                                            {-7.51266, 5.67161, -0.345191},//46

                                            {-7.51266, 5.67161, -0.345191},//47
                                            {-7.7836, 5.69357, -0.33855}//47
                                    }
    };
    for (int ii1 = 0; ii1 < biases.size(); ii1++) { modelsIndexes.emplace_back(); }
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
    for (int ii1 = 0; ii1 < biases.size(); ii1++) {
        subBonesArray.emplace_back();
        subInit(biases[ii1], model, subBonesArray[ii1], modelsIndexes[ii1]);
        for (int ii2 = 0; ii2 < modelsIndexes[ii1].size(); ii2++)
            skinWModel(model, modelsIndexes[ii1][ii2], subBonesArray[ii1]);
        clap(bonesArray, subBonesArray[ii1], b);
    }
}
