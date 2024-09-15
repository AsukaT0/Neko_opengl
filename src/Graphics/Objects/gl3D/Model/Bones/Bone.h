//
// Created by asuka on 28.05.2024.
//

#ifndef MIRAI_BONE_H
#define MIRAI_BONE_H


#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../../../../Math/Structs.h"
#include "../../Camera/PerspectiveCamera.h"
#include "../Model3D/Model.h"

class Bone {
private:
    std::vector<BoneData> data0;
    std::vector<Bone> allBones;
    s2i modelKeys;
    int parent = -1;
    std::vector<int> childs;
    Vertex
        start{0,0,0},
        end{0,0,0},
        rotate{0,0,0};
    Color color{};
    Model *p;
    std::vector<Bone> *boneArr;
public:
    Bone();
    Bone(Model&part,const std::vector<std::string>&modelIDs);
    Bone(Model&part);
    Bone(Model&part,const std::vector<int>&childs,const std::vector<std::string>& modelIDs);
    Bone(Model&part,const std::vector<int>&childs);
    void addChild(int index);

    void setModelIDs(const std::vector<std::string>&modelIDs);
    void setModelPart(Model*part);
    void setParent(int index);
    void setPos(Vertex v1,Vertex v2);
    void setBonesArray(std::vector<Bone> *bones);

    [[nodiscard]] int getParent() const;
    [[nodiscard]] std::vector<int> getChilds() const;
    std::vector<BoneData> getData();

    void shiftChilds(int codeshifter);
    void shiftParent(int codeshifter);
    void shift(int codeshifter);

    void addIndices(const std::string& tag,int indice,float weight);
    void render();

    void renderP(Model model);

    Vertex getStart();
    Vertex getEnd();

    void translateX(float dist);
    void translateY(float dist);
    void translateZ(float dist);

    void setRotationX(const Vertex &center, const float &angle);
    void setRotationY(const Vertex &center, const float &angle);
    void setRotationZ(const Vertex &center, const float &angle);

    void setRotationX(const float &angle);
    void setRotationY(const float &angle);
    void setRotationZ(const float &angle);

    void rotAPX(const Vertex &center, const float &angle);
    void rotAPZ(const Vertex &center, const float &angle);
    void rotAPY(const Vertex &center, const float &angle);

    void degRAPX(const Vertex &center, float angle);
    void degRAPY(const Vertex &center, float angle);
    void degRAPZ(const Vertex &center, float angle);


    [[maybe_unused]] void rotX(float angle);
    [[maybe_unused]] void rotY(float angle);
    [[maybe_unused]] void rotZ(float angle);
    void funRAPX(const Vertex &center, float currentAngle, const std::function<float(float, float, float)> &function,
                 const float &start_a, const float &iter);
    void funRAPY(const Vertex &center, float currentAngle, const std::function<float(float, float, float)> &function,
                 const float &start_a, const float &iter);
    void funRAPZ(const Vertex &center, float currentAngle, const std::function<float(float, float, float)> &function,
                 const float &start_a, const float &iter);

    void destroy();
    void reset();
};


#endif //MIRAI_BONE_H
