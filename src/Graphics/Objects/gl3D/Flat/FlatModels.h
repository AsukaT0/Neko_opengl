//
// Created by asuka on 22.02.2024.
//

#ifndef MIRAAI_FLATMODELS_H
#define MIRAAI_FLATMODELS_H


#include <vector>
#include "../../../Math/Structs.h"
#include "../BaseModel.h"

class FlatModels : public BaseModel{
private:
    std::vector<Vector3D > dots;
    std::vector<FlatModels> modelParts;
public:
    explicit FlatModels(std::vector<Vector3D > pointVector);
    void render() override;
    [[maybe_unused]] void addModelPart(FlatModels model);
    [[maybe_unused]] FlatModels getModelPart(int index);
    [[maybe_unused]] void deleteModelPart(int index);
};


#endif //MIRAAI_FLATMODELS_H
