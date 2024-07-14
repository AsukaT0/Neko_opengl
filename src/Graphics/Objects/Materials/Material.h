//
// Created by asuka on 09.05.2024.
//

#ifndef MIRAAI_MATERIAL_H
#define MIRAAI_MATERIAL_H


#include <string>
#include "../../Math/Structs.h"
#include "../gl2D/Texture.h"

class Material {
private:
    double Ns,d,Ni;
    Color Ka,Kd,Ks,Ke;
    int illum;
    Texture texture;
public:
    Material(const double& Ns,
             const Color& Ka,
             const Color& Kd,
             const Color& Ks,
             const Color& Ke,
             const double& Ni,
             const double& d,
             const int& illum);
    Material(const double& Ns,
             const Color& Ka,
             const Texture& Kd,
             const Color& Ks,
             const Color& Ke,
             const double& Ni,
             const double& d,
             const int& illum);
    double getNs();
    Color getKa();
    Color getKdColor();
    Texture getKdTexture();
    Color getKs();
    Color getKe();
    double getNi();
    double getD();
    int getIllum();
    std::string toString();

    Material();
    void destroy();
};


#endif //MIRAAI_MATERIAL_H
