//
// Created by asuka on 28.05.2024.
//

#include <iostream>
#include "Bone.h"
#include <cmath>

Bone::Bone() {

}

Color getRand() {
    return {(float) ((rand() % 255) / 255.0), (float) ((rand() % 255) / 255.0), (float) ((rand() % 255) / 255.0)};
}

Bone::Bone(Model &part, const std::vector<int> &childs, const std::vector<std::string> &modelIDs) {
    p = &part;
    data0 = {};
    for (const std::string &s: modelIDs) {
        modelKeys[s] = data0.size();
        data0.emplace_back(s);
    }
    this->childs = childs;
    color = getRand();
}

Bone::Bone(Model &part, const std::vector<std::string> &modelIDs) {
    p = &part;
    data0 = {};
    for (const std::string &s: modelIDs) {
        modelKeys[s] = data0.size();
        data0.emplace_back(s);
    }
    color = getRand();
}

void Bone::setPos(Vertex v1, Vertex v2) {
    start = v1;
    end = v2;
}

void Bone::render() {
    glLineWidth(2.0);
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_LINES);

    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}

void Bone::addIndices(const std::string &tag, int indice, float weight) {
    if (modelKeys.contains(tag))
        data0[modelKeys[tag]].weightAndIndexArray.emplace_back(indice, weight);
}

Vertex Bone::getStart() { return start; }

Vertex Bone::getEnd() { return end; }

void Bone::renderP(Model model) {
    glBegin(GL_POINTS);
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            vertarr &vert = model.getModelPartByTag(bd.modelID).getVertex();
            glVertex3f(vert[a.i].x, vert[a.i].y, vert[a.i].z);
        }
    glEnd();
}

void Bone::translateX(float dist) {
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {

            p->getModelPartByTag(bd.modelID).getVertex()[a.i].x += dist;
        }
    for (int ch: childs) { (*boneArr)[ch].translateX(dist); }
}

void Bone::translateY(float dist) {
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            p->getModelPartByTag(bd.modelID).getVertex()[a.i].y += dist;
        }
    for (int ch: childs) { (*boneArr)[ch].translateY(dist); }
}


void Bone::translateZ(float dist) {
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            p->getModelPartByTag(bd.modelID).getVertex()[a.i].z += dist;
        }
    for (int ch: childs) { (*boneArr)[ch].translateZ(dist); }
}

void totetoX(Vertex &v, const Vertex &center, const float &cosAngle, const float &sinAngle) {
    float tempY, tempZ;
    asm volatile(
            "movss %[vy], %%xmm2\n\t"
            "subss %[centerY], %%xmm2\n\t"  // xmm2 = tempY = vy - centerY
            "movss %[vz], %%xmm3\n\t"
            "subss %[centerZ], %%xmm3\n\t"  // xmm3 = tempZ = vz - centerZ

            "movaps %%xmm2, %%xmm4\n\t"    // xmm4 = tempY
            "movaps %%xmm3, %%xmm5\n\t"    // xmm5 = tempZ

            "mulss %[cosAngle], %%xmm2\n\t" // xmm2 = tempY * cosAngle
            "mulss %[sinAngle], %%xmm3\n\t" // xmm3 = tempZ * sinAngle
            "subss %%xmm3, %%xmm2\n\t"      // xmm2 = tempY * cosAngle - tempZ * sinAngle
            "addss %[centerY], %%xmm2\n\t"  // v.y = xmm2 + centerY
            "movss %%xmm2, %[vy]\n\t"

            "mulss %[sinAngle], %%xmm4\n\t" // xmm4 = tempY * sinAngle
            "mulss %[cosAngle], %%xmm5\n\t" // xmm5 = tempZ * cosAngle
            "addss %%xmm5, %%xmm4\n\t"      // xmm4 = tempY * sinAngle + tempZ * cosAngle
            "addss %[centerZ], %%xmm4\n\t"  // v.z = xmm4 + centerZ
            "movss %%xmm4, %[vz]\n\t"
            :
    [tempY]"=m"(tempY), [tempZ]"=m"(tempZ)
    :[vy]"m"(v.y), [vz]"m"(v.z),
    [centerY]"m"(center.y), [centerZ]"m"(center.z),
    [cosAngle]"m"(cosAngle), [sinAngle]"m"(sinAngle)
    :"xmm2", "xmm3", "xmm4", "xmm5", "memory"
    );
}

void tetoteY(Vertex &v, const Vertex &center, const float &val1, const float &val2) {
    float temp1, temp2;
    asm volatile(
            "movss %[vx], %%xmm2\n\t"
            "subss %[center1], %%xmm2\n\t"  // xmm2 = temp1 = vx - center1
            "movss %[vz], %%xmm3\n\t"
            "subss %[center2], %%xmm3\n\t"  // xmm3 = temp2 = vz - centerZ

            "movaps %%xmm2, %%xmm4\n\t"    // xmm4 = temp1
            "movaps %%xmm3, %%xmm5\n\t"    // xmm5 = temp2

            "mulss %[val1], %%xmm2\n\t" // xmm2 = temp1 * val1
            "mulss %[val2], %%xmm3\n\t" // xmm3 = temp2 * val2
            "addss %%xmm3, %%xmm2\n\t"      // xmm2 = temp1 * val1 + temp2 * val2
            "addss %[center1], %%xmm2\n\t"  // v.x = xmm2 + center1
            "movss %%xmm2, %[vx]\n\t"

            "mulss %[val1], %%xmm5\n\t" // xmm5 = temp2 * val1
            "mulss %[val2], %%xmm4\n\t" // xmm4 = temp1 * val2
            "subss %%xmm4, %%xmm5\n\t"      // xmm5 = temp2 * val1 - temp1 * val2
            "addss %[center2], %%xmm5\n\t"  // v.z = xmm5 + centerZ
            "movss %%xmm5, %[vz]\n\t"
            :
    [temp1]"=m"(temp1), [temp2]"=m"(temp2)
    :[vx]"m"(v.x), [vz]"m"(v.z),
    [center1]"m"(center.x), [center2]"m"(center.z),
    [val1]"m"(val1), [val2]"m"(val2)
    :"xmm2", "xmm3", "xmm4", "xmm5", "memory"
    );
}


void tetoZ(Vertex &v, const Vertex &center, const float &val1, const float &val2) {
    float temp1, temp2;
    asm volatile(
        // compute temp1 = v.x - center.x
            "movss %[vx], %%xmm2\n\t"
            "subss %[center1], %%xmm2\n\t"  // xmm2 = temp1 = vx - center1
            "movss %[vy], %%xmm3\n\t"
            "subss %[center2], %%xmm3\n\t"  // xmm3 = temp2 = vy - centerY

            "movaps %%xmm2, %%xmm4\n\t"    // xmm4 = temp1
            "movaps %%xmm3, %%xmm5\n\t"    // xmm5 = temp2

            "mulss %[val1], %%xmm2\n\t" // xmm2 = temp1 * val1
            "mulss %[val2], %%xmm3\n\t" // xmm3 = temp2 * val2
            "subss %%xmm3, %%xmm2\n\t"      // xmm2 = temp1 * val1 - temp2 * val2
            "addss %[center1], %%xmm2\n\t"  // vx = xmm2 + center1
            "movss %%xmm2, %[vx]\n\t"

            "mulss %[val2], %%xmm4\n\t" // xmm4 = temp1 * val2
            "mulss %[val1], %%xmm5\n\t" // xmm5 = temp2 * val1
            "addss %%xmm5, %%xmm4\n\t"      // xmm4 = temp1 * val2 + temp2 * val1
            "addss %[center2], %%xmm4\n\t"  // vy = xmm4 + centerY
            "movss %%xmm4, %[vy]\n\t"
            :
    [temp1]"=m"(temp1), [temp2]"=m"(temp2)
    :[vx]"m"(v.x), [vy]"m"(v.y),
    [center1]"m"(center.x), [center2]"m"(center.y),
    [val1]"m"(val1), [val2]"m"(val2)
    :"xmm2", "xmm3", "xmm4", "xmm5", "memory"
    );
}

[[maybe_unused]] void Bone::rotAPX(const Vertex &center, const float &angle) {
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(angle * a.w);
            float sinAngle = std::sin(angle * a.w);
//            std::cout << bd.modelID << "\n";
            totetoX(p->getModelPartByTag(bd.modelID).getVertex()[a.i],
                    center, cosAngle, sinAngle);
        }
    for (int ch: childs) { (*boneArr)[ch].rotAPX(center, angle); }
}


void Bone::rotAPY(const Vertex &center, const float &angle) {
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(angle * a.w);
            float sinAngle = std::sin(angle * a.w);
            Model::ModelPart &modelPart = p->getModelPartByTag(bd.modelID);
            tetoteY(modelPart.getVertex()[a.i],
                    center, cosAngle, sinAngle);
        }
    for (int ch: childs) {
        (*boneArr)[ch].rotAPY(center, angle); }
}

void Bone::rotAPZ(const Vertex &center, const float &angle) {
    for (const BoneData &bd: data0) {
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(angle * a.w);
            float sinAngle = std::sin(angle * a.w);
            Model::ModelPart &modelPart = p->getModelPartByTag(bd.modelID);
            tetoZ(modelPart.getVertex()[a.i],
                  center, cosAngle, sinAngle);
        }
    }
    for (int ch: childs) {
        (*boneArr)[ch].rotAPZ(center, angle);
    }
}

void Bone::funRAPX(const Vertex &center, float currentAngle, const std::function<float(float, float, float)> &function,
                   const float &start_a, const float &iter) {
    float angle = function(start_a, iter, 0) + currentAngle;
    for (const BoneData &bd: data0) {
        Model::ModelPart &modelPart = p->getModelPartByTag(bd.modelID);
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(angle * a.w);
            float sinAngle = std::sin(angle * a.w);
            totetoX(modelPart.getVertex()[a.i],
                    center, cosAngle, sinAngle);
        }
    }
    for (int ch: childs) { (*boneArr)[ch].funRAPX(center, angle, function, start_a, iter + 1); }
}

void Bone::funRAPY(const Vertex &center, float currentAngle, const std::function<float(float, float, float)> &function,
                   const float &start_a, const float &iter) {
    float angle = function(start_a, iter, 0) + currentAngle;
    for (const BoneData &bd: data0) {
        Model::ModelPart &modelPart = p->getModelPartByTag(bd.modelID);
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(angle * a.w);
            float sinAngle = std::sin(angle * a.w);
            tetoZ(modelPart.getVertex()[a.i],
                  center, cosAngle, sinAngle);
        }
    }
    for (int ch: childs) { (*boneArr)[ch].funRAPY(center, angle, function, start_a, iter + 1); }
}

void
Bone::funRAPZ(const Vertex &center, float currentAngle, const std::function<float(float, float, float)> &function,
              const float &start_a = 0, const float &iter = 0) {
    float angle = function(start_a, iter, 0) + currentAngle;
    for (const BoneData &bd: data0) {
        Model::ModelPart &modelPart = p->getModelPartByTag(bd.modelID);
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(angle * a.w);
            float sinAngle = std::sin(angle * a.w);
            tetoZ(modelPart.getVertex()[a.i],
                  center, cosAngle, sinAngle);
        }
    }
    for (int ch: childs) { (*boneArr)[ch].funRAPZ(center, angle, function, start_a, iter + 1); }
}

void Bone::destroy() {
    delete p;
    childs.clear();
    data0.clear();
}

void Bone::reset() {
    for (int ch: childs) {
        (*boneArr)[ch].reset();
    }
    for (const BoneData &bd: data0) {
        p->getModelPartByTag(bd.modelID).reset();
    }
}

void Bone::rotX(float angle) {
    rotAPX(start, angle);
}

void Bone::rotY(float angle) {
    rotAPY(start, angle);
}

void Bone::rotZ(float angle) {
    rotAPZ(start, angle);
}


void Bone::degRAPX(const Vertex &center, float angle) {
    rotAPX(center, glm::radians(angle));
}

void Bone::degRAPY(const Vertex &center, float angle) {
    rotAPY(center, glm::radians(angle));
}

void Bone::degRAPZ(const Vertex &center, float angle) {
    rotAPZ(center, glm::radians(angle));
}

void Bone::setParent(int index) { parent = index; }

int Bone::getParent() const { return parent; }

void Bone::shiftParent(int codeshifter) { if (parent != -1)parent += codeshifter; }

void Bone::shiftChilds(int codeshifter) {
    for (int &boneI: childs) {
        boneI += codeshifter;
    }
}

void Bone::addChild(int index) { childs.push_back(index); }

std::vector<int> Bone::getChilds() const { return childs; }

void Bone::shift(int codeshifter) {
    shiftChilds(codeshifter);
    shiftParent(codeshifter);
}

std::vector<BoneData> Bone::getData() { return data0; }

void Bone::setRotationX(const Vertex &center, const float &angle) {
    float realDegree = angle - rotate.x;
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(realDegree * a.w);
            float sinAngle = std::sin(realDegree * a.w);
            totetoX(p->getModelPartByTag(bd.modelID).getVertex()[a.i],
                    center, cosAngle, sinAngle);
        }
    rotate.x = angle;
    for (int ch: childs) { (*boneArr)[ch].setRotationX(center, angle); }
}

void Bone::setRotationY(const Vertex &center, const float &angle) {
    float realDegree = angle - rotate.y;
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(realDegree * a.w);
            float sinAngle = std::sin(realDegree * a.w);
            tetoteY(p->getModelPartByTag(bd.modelID).getVertex()[a.i],
                    center, cosAngle, sinAngle);
        }
    rotate.y = angle;
    for (int ch: childs) { (*boneArr)[ch].setRotationY(center, angle); }
}

void Bone::setRotationZ(const Vertex &center, const float &angle) {
    float realDegree = angle - rotate.z;
    for (const BoneData &bd: data0)
        for (IW a: bd.weightAndIndexArray) {
            float cosAngle = std::cos(realDegree * a.w);
            float sinAngle = std::sin(realDegree * a.w);
            tetoZ(p->getModelPartByTag(bd.modelID).getVertex()[a.i],
                  center, cosAngle, sinAngle);
        }
    rotate.z = angle;
    for (int ch: childs) { (*boneArr)[ch].setRotationZ(center, angle); }
}

void Bone::setRotationX(const float &angle) {
    setRotationX(start, angle);
}

void Bone::setRotationY(const float &angle) {
    setRotationY(start, angle);
}

void Bone::setRotationZ(const float &angle) {
    setRotationZ(start, angle);
}

Bone::Bone(Model &part) {
    p = &part;
    color = getRand();
}

Bone::Bone(Model &part, const std::vector<int> &childs) {
    p = &part;
    color = getRand();
    this->childs = childs;

}

void Bone::setModelPart(Model *part) {
    p = part;
}

void Bone::setModelIDs(const std::vector<std::string> &modelIDs) {
    data0 = {};
    for (const std::string &s: modelIDs) {
        modelKeys[s] = data0.size();
        data0.emplace_back(s);
    }

}

void Bone::setBonesArray(std::vector<Bone> *bones) {
    boneArr = bones;
}
