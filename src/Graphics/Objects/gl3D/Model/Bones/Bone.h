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
public:
    Bone();
    Bone(Model&part,const std::vector<std::string>&modelIDs);
    Bone(Model&part);
    Bone(Model&part,const std::vector<int>&childs,const std::vector<std::string>& modelIDs);
    Bone(Model&part,const std::vector<int>&childs);
    void setPos(Vertex v1,Vertex v2);

    void addChild(int index);
    [[nodiscard]] std::vector<int> getChilds() const;

    void setParent(int index);
    [[nodiscard]] int getParent() const;

    void shiftChilds(int codeshifter);
    void shiftParent(int codeshifter);
    void shift(int codeshifter);

    void addIndices(const std::string& tag,int indice,float weight);
    void render();

    void renderP(Model model);

    Vertex getStart();
    Vertex getEnd();

    void translateX(std::vector<Bone>&bone,float dist);
    void translateY(std::vector<Bone>&bone,float dist);
    void translateZ(std::vector<Bone>&bone,float dist);

    void setRotationX(Model&part, std::vector<Bone>& bone, const Vertex& center, const float& angle);
    void setRotationY(Model&part, std::vector<Bone>& bone, const Vertex& center, const float& angle);
    void setRotationZ(Model&part, std::vector<Bone>& bone, const Vertex& center, const float& angle);

    void setRotationX(Model&part, std::vector<Bone>& bone, const float& angle);
    void setRotationY(Model&part, std::vector<Bone>& bone, const float& angle);
    void setRotationZ(Model&part, std::vector<Bone>& bone, const float& angle);

    void rotAPX(Model&part, std::vector<Bone>& bone, const Vertex& center, const float& angle);
    void rotAPY(Model&part, std::vector<Bone>& bone, const Vertex& center, const float& angle);
    void rotAPZ(Model&part, std::vector<Bone>& bone, const Vertex& center, const float& angle);
    void degRAPX(Model&part, std::vector<Bone>& bone, const Vertex& center, float angle);
    void degRAPY(Model&part, std::vector<Bone>& bone, const Vertex& center, float angle);
    void degRAPZ(Model&part, std::vector<Bone>& bone, const Vertex& center, float angle);
    [[maybe_unused]] void rotAPX(std::vector<Bone>& bone, const Vertex& center, float angle);
    [[maybe_unused]] void rotAPY(std::vector<Bone>& bone, const Vertex& center, float angle);
    [[maybe_unused]] void rotAPZ(std::vector<Bone>& bone, const Vertex& center, float angle);

    [[maybe_unused]] void rotX(Model&part, std::vector<Bone>&bone, float angle);
    [[maybe_unused]] void rotY(Model&part, std::vector<Bone>&bone, float angle);
    [[maybe_unused]] void rotZ(Model&part, std::vector<Bone>&bone, float angle);
    [[maybe_unused]] void rotX(std::vector<Bone>&bone, float angle);
    [[maybe_unused]] void rotY(std::vector<Bone>&bone, float angle);
    [[maybe_unused]] void rotZ(std::vector<Bone>&bone, float angle);
    void funRAPX(Model&part, std::vector<Bone>& bone, const Vertex &center,float currentAngle, const std::function<float (float start, float iter, float step)>& function, const float& start_a, const float& iter);
    void funRAPY(Model&part, std::vector<Bone>& bone, const Vertex &center,float currentAngle, const std::function<float (float start, float iter, float step)>& function, const float& start_a, const float& iter);
    void funRAPZ(Model&part, std::vector<Bone>& bone, const Vertex &center,float currentAngle, const std::function<float (float start, float iter, float step)>& function, const float& start_a, const float& iter);

    void destroy();
    void reset(Model&part, std::vector<Bone>&bone);
    void setModelIDs(const std::vector<std::string>&modelIDs);
    void setModelPart(Model&part);
    std::vector<BoneData> getData();
};


#endif //MIRAI_BONE_H
