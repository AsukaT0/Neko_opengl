//
// Created by asuka on 01.06.2024.
//

#ifndef MIRAI_ANIMATIONCONTROLLER_H
#define MIRAI_ANIMATIONCONTROLLER_H


#include <vector>
#include "../Model/Model3D/Model.h"
#include "../Model/Bones/Bone.h"
#include "BoneAnimation.h"

class AnimationController {
private:
    std::vector<BoneAnimation> anims;
    std::map<std::string,int> tagger;
public:
    void startAnim(int index);
    void startAll();
    void render(std::vector<Bone> &bonesArray, Model &model1);
    std::vector<BoneAnimation>& getAnimationsList();
    void addAnim(const std::string& tag,const BoneAnimation& anim);
    void destroy();
};


#endif //MIRAI_ANIMATIONCONTROLLER_H
