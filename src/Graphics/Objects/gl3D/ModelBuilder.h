//
// Created by asuka on 22.02.2024.
//

#ifndef MIRAAI_MODELBUILDER_H
#define MIRAAI_MODELBUILDER_H


#include "Flat/FlatModels.h"

class ModelBuilder {
public:
    [[maybe_unused]] static FlatModels genCircleXY(float radius, int segmentsNum);
    [[maybe_unused]] static FlatModels genCircleXZ(float radius, int segmentsNum);
    [[maybe_unused]] static FlatModels genCircleYZ(float radius, int segmentsNum);
    [[maybe_unused]] static FlatModels genGridXY(int gridSize,float gridSpacing);
    [[maybe_unused]] static FlatModels genGridXZ(int gridSize,float gridSpacing);
    [[maybe_unused]] static FlatModels genGridYZ(int gridSize,float gridSpacing);
    [[maybe_unused]] static FlatModels genGrids(int gridSize,float gridSpacing);
    [[maybe_unused]] static FlatModels genCircles(float radius, int segmentsNum);
};


#endif //MIRAAI_MODELBUILDER_H
