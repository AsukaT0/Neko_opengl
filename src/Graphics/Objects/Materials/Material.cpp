//
// Created by asuka on 09.05.2024.
//

#include "Material.h"

Material::Material(const double& Ns,const Color& Ka, const Color& Kd, const Color& Ks, const Color& Ke,const double& Ni, const double& d, const int& illum) {
    this->Ns = Ns;
    this->Ka = Ka;
    this->Kd = Kd;
    this->Ks = Ks;
    this->Ke = Ke;
    this->Ni = Ni;
    this->d = d;
    this->illum = illum;
}

Material::Material(const double &Ns, const Color &Ka, const Texture &Kd, const Color &Ks, const Color &Ke,
                   const double &Ni, const double &d, const int &illum) {
    this->Ns = Ns;
    this->Ka = Ka;
    this->texture = Kd;
    this->Ks = Ks;
    this->Ke = Ke;
    this->Ni = Ni;
    this->d = d;
    this->illum = illum;
}
double Material::getNs() {return Ns;}
Color Material::getKa() {return Ka;}
Color Material::getKdColor() {return Kd;}
Color Material::getKs() {return Ks;}
Color Material::getKe() {return Ke;}
double Material::getNi() {return Ni;}
double Material::getD() {return d;}
int Material::getIllum() {return illum;}
Texture Material::getKdTexture() {return texture;}

std::string Material::toString() {
    std::string maybe_baby = "";
    maybe_baby += "Ns:"+std::to_string(Ns)+"\n";
    maybe_baby += "Ka:"+std::to_string(Ka.r)+","+std::to_string(Ka.g)+","+std::to_string(Ka.b)+"\n";
    maybe_baby += "Kd:"+std::to_string(Kd.r)+","+std::to_string(Kd.g)+","+std::to_string(Kd.b)+"\n";
    maybe_baby += "Ks:"+std::to_string(Ks.r)+","+std::to_string(Ks.g)+","+std::to_string(Ks.b)+"\n";
    maybe_baby += "Ke:"+std::to_string(Ke.r)+","+std::to_string(Ke.g)+","+std::to_string(Ke.b)+"\n";
    maybe_baby += "Ni:"+std::to_string(Ni)+"\n";
    maybe_baby += "d:"+std::to_string(d)+"\n";
    maybe_baby += "illum:"+std::to_string(illum)+"\n";
    return maybe_baby;
}

Material::Material() {}

void Material::destroy() {
    texture.destroy();
}
