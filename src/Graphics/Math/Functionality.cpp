//
// Created by asuka on 05.05.2024.
//

#include "Functionality.h"
#include <cmath>

GLfloat Functionality::scalar2Points(Vector3D point1, Vector3D point2) {
    return sqrtf(point1.x * point2.x + point1.y * point2.y + point1.z * point2.z);
}

GLfloat Functionality::scalar2Points(Vertex point1, Vertex point2) {
    return sqrtf(point1.x * point2.x + point1.y * point2.y + point1.z * point2.z);
}

GLfloat Functionality::scalar2Points(Normal point1, Normal point2) {
    return sqrtf(point1.x * point2.x + point1.y * point2.y + point1.z * point2.z);
}

GLfloat Functionality::dist2tweenP(const Vector3D &P1, const Vector3D &P2) {
    return sqrtf(pow(P1.x - P2.x,2)+pow(P1.y - P2.y,2)+pow(P1.z - P2.z,2));}

GLfloat Functionality::dist2tweenP(const Vertex &P1, const Vertex &P2) {
    return sqrtf(pow(P1.x - P2.x,2)+pow(P1.y - P2.y,2)+pow(P1.z - P2.z,2));}

GLfloat Functionality::distancePointToLine(const Vector3D &P, const Vector3D &P1, const Vector3D &P2) {
    Vector3D V = {P2.x - P1.x, P2.y - P1.y, P2.z - P1.z};
    Vector3D W = {P.x - P1.x, P.y - P1.y, P.z - P1.z};
    Vector3D Cross = {
            V.y * W.z - V.z * W.y,
            V.z * W.x - V.x * W.z,
            V.x * W.y - V.y * W.x};
    GLfloat Magnitude = scalar2Points(Cross,Cross);
    GLfloat VMagnitude = scalar2Points(V,V);
    GLfloat Distance = Magnitude / VMagnitude;
    return Distance;
}

std::vector<Vertex> Functionality::exPByNorms(const std::vector<Vertex> &vert, const std::vector<Normal> &norm,
                                              const std::vector<GLuint> &indi, float factor) {
    std::vector<Vertex> expandedVertices;
    expandedVertices.resize(vert.size());
    for (int indice : indi){
        glm::vec3 v0 = reinterpret_cast<const glm::vec3 &>(vert[indice]);
        glm::vec3 n0 = reinterpret_cast<const glm::vec3 &>(norm[indice]);
        glm::vec3 newV = v0+factor*n0;
        expandedVertices[indice] = {newV.x,newV.y,newV.z};}
    return expandedVertices;
}
double Functionality::distToSeg(Vertex p, Vertex l1, Vertex l2) {
    Vertex v = l2 - l1;
    Vertex w = p - l1;
    double proj = (w.x * v.x + w.y * v.y + w.z * v.z) / (v.x * v.x + v.y * v.y + v.z * v.z);
    if (proj < 0) {
        return scalar2Points(w,w);}
    else if (proj > 1) {
        return sqrtf((p.x - l2.x) * (p.x - l2.x) + (p.y - l2.y) * (p.y - l2.y) + (p.z - l2.z) * (p.z - l2.z));}
    else {
        Vertex l = l1+v*proj;
        return sqrtf((p.x - l.x) * (p.x - l.x) + (p.y - l.y) * (p.y - l.y) + (p.z - l.z) * (p.z - l.z));}
}
