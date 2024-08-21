//
// Created by asuka on 05.05.2024.
//

#include "Structs.h"
#ifndef MIRAAI_FUNCTIONALITY_H


#define MIRAAI_FUNCTIONALITY_H
#include <vector>

class Functionality {
private:
public:
    static GLfloat scalar2Points(Vector3D point1, Vector3D point2);
    static GLfloat scalar2Points(Vertex point1, Vertex point2);
    static GLfloat scalar2Points(Normal point1, Normal point2);
    static GLfloat dist2tweenP(const Vector3D& P1, const Vector3D& P2);
    static GLfloat dist2tweenP(const Vertex& P1, const Vertex& P2);
    static GLfloat distancePointToLine(const Vector3D& P, const Vector3D& P1, const Vector3D& P2);
    static std::vector<Vertex> exPByNorms(const std::vector<Vertex>&vert, const std::vector<Normal> &norm,
                                          const std::vector<GLuint>&indi, float factor);
    static double distToSeg(Vertex p, Vertex l1, Vertex l2);
    static float calculateAngle(Vertex A, Vertex B, Vertex C, Vertex D);
    static bool intersectLineBox(const Vertex& lineStart, const Vertex& lineEnd,
                          const Vertex& boxMin, const Vertex& boxMax);
};


#endif //MIRAAI_FUNCTIONALITY_H
