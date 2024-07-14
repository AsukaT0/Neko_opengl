//
// Created by asuka on 22.02.2024.
//

#include <cmath>
#include "ModelBuilder.h"

[[maybe_unused]] FlatModels ModelBuilder::genCircleXY(float radius, int numSegments) {
    std::vector<Vector3D > dots;
    for (int i = 0; i < numSegments; ++i)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        float z = 0.0f;
        dots.emplace_back(x, y, z);
    }
    return FlatModels(dots);
}
[[maybe_unused]] FlatModels ModelBuilder::genCircleXZ(float radius, int numSegments) {
    std::vector<Vector3D> dots;
    for (int i = 0; i < numSegments; ++i)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radius * cos(theta);
        float y = 0.0f; // Окружность лежит на плоскости XZ
        float z = radius * sin(theta);
        dots.emplace_back(x, y, z);
    }
    return FlatModels(dots);
}
[[maybe_unused]] FlatModels ModelBuilder::genCircleYZ(float radius, int numSegments) {
    std::vector<Vector3D> dots;
    for (int i = 0; i < numSegments; ++i)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = 0.0f;
        float y = radius * cos(theta);
        float z = radius * sin(theta);
        dots.emplace_back(x, y, z);
    }
    return FlatModels(dots);
}

[[maybe_unused]] FlatModels ModelBuilder::genGridXY(int gridSize, float gridSpacing) {
    std::vector<Vector3D > dots;
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        dots.emplace_back(gridSize,  i, 0.0f);
        dots.emplace_back(-gridSize, i, 0.0f);
    }
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        dots.emplace_back(i, -gridSize, 0.0f);
        dots.emplace_back(i, gridSize, 0.0f);
    }
    return FlatModels(dots);
}

[[maybe_unused]] FlatModels ModelBuilder::genGridXZ(int gridSize, float gridSpacing) {
    std::vector<Vector3D > dots;
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        dots.emplace_back(i, 0.0f, -gridSize);
        dots.emplace_back(i, 0.0f, gridSize);
    }
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        dots.emplace_back(gridSize, 0.0f, i);
        dots.emplace_back(-gridSize, 0.0f, i);
    }
    return FlatModels(dots);
}

[[maybe_unused]] FlatModels ModelBuilder::genGridYZ(int gridSize, float gridSpacing) {
    std::vector<Vector3D > dots;
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        dots.emplace_back(0.0f,-gridSize,  i);
        dots.emplace_back(0.0f,gridSize, i);}
    for (float i = -gridSize; i <= gridSize; i += gridSpacing) {
        dots.emplace_back(0.0f,i,  gridSize);
        dots.emplace_back(0.0f,i,  -gridSize);}
    return FlatModels(dots);
}

[[maybe_unused]] FlatModels ModelBuilder::genGrids(int gridSize, float gridSpacing) {
    FlatModels models({});
    models.addModelPart(genGridXY(gridSize,gridSpacing));
    models.addModelPart(genGridXZ(gridSize,gridSpacing));
    models.addModelPart(genGridYZ(gridSize,gridSpacing));
    return models;}

FlatModels ModelBuilder::genCircles(float radius, int segmentsNum) {
    FlatModels models({});
    models.addModelPart(genCircleXY(radius,segmentsNum));
    models.addModelPart(genCircleYZ(radius,segmentsNum));
    models.addModelPart(genCircleXZ(radius,segmentsNum));
    return models;
}
