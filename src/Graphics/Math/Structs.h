//
// Created by asuka on 22.02.2024.
//

#ifndef MIRAAI_STRUCTS_H
#define MIRAAI_STRUCTS_H

#include <GL/glew.h>
#include <cmath>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include <map>

#define s2i std::map<std::string,int>
#define vertarr std::vector<Vertex>

class Normal {
public:
    Normal() = default;
    GLfloat x, y, z;
    Normal(GLfloat d, GLfloat d1, GLfloat d2){
        x = d;
        y = d1;
        z = d2;}
    explicit Normal(glm::vec3 vec3){
        x = vec3.x;
        y = vec3.y;
        z = vec3.z;}
    void normalize() {
        GLfloat length = std::sqrt(x * x + y * y + z * z);
        x = x / length;
        y = y / length;
        z = z / length;
    };
    Normal operator+(const Normal& other) {
        Normal result{x+=other.x,y+=other.y,z+=other.z};
        return result;}
    Normal operator-(const Normal& other) {
        Normal result{x-=other.x,y-=other.y,z-=other.z};
        return result;}
    Normal operator+=(const Normal& other) {
        Normal result{x+=other.x,y+=other.y,z+=other.z};
        return result;}
    Normal operator-=(const Normal& other) {
        Normal result{x-=other.x,y-=other.y,z-=other.z};
        return result;}
    Normal operator/(const float & other) {
        Normal result{x/=other,y/=other,z/=other};
        return result;}
    Normal operator/=(const float & other) {
        Normal result{x/=other,y/=other,z/=other};
        return result;}

};

class Vertex;

struct Vertex {
    GLfloat x, y, z;
    Vertex operator+(const Vertex& other) const {
        Vertex result{ x + other.x, y + other.y, z + other.z };
        return result;
    }
    Vertex operator+=(const Vertex& other){
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vertex operator-=(const Vertex& other){
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vertex operator/=(const float &scalar){
        x/=scalar;
        y/=scalar;
        z/=scalar;
        return *this;
    }
    Vertex operator-(const Vertex& other) const {
        Vertex result{ x - other.x, y - other.y, z - other.z };
        return result;}
    Vertex operator*(const float &scalar)const{
        return Vertex{x*scalar,y*scalar,z*scalar};}
    Vertex operator/(const float &scalar)const{
        return Vertex{x/scalar,y/scalar,z/scalar};}
    bool operator==(const Vertex& other)const{
        return (x==other.x)&&(y==other.y)&&(z==other.z);}
};

struct Vector3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    Vector3D operator=(const Vertex& other) const {
        Vector3D result{ other.x, other.y, other.z };
        return result;
    }
};

struct TextureCoord {
    GLfloat u, v;
    bool operator==(const TextureCoord& other) const {
        return (u == other.u)&&(v == other.v);
    }
};
struct Color{
    GLfloat r = 0.0f;
    GLfloat g = 0.0f;
    GLfloat b = 0.0f;
    GLfloat a = 1.0f;
};
struct IW{
    int i;
    float w;
};
struct BoneData{
    std::string modelID;
    std::vector<IW> weightAndIndexArray;
};
#endif //MIRAAI_STRUCTS_H
