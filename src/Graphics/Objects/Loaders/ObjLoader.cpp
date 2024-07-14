#include <fstream>
#include <iostream>
#include "ObjLoader.h"
#include <Utils/StringUtils.h>
#include <sstream>
#include <Utils/SwitcherString.h>
#include "../../Math/Functionality.h"
#include "../Materials/Material.h"
#include "../../../Controller.h"

std::vector<std::string> ObjLoader::lines;
Model ObjLoader::load(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open OBJ file: " << path << std::endl;
        return Model{};}
    std::vector<Vertex> tempVertices;
    std::vector<Normal> tempNormals;
    std::vector<Material> tempColors;
    std::vector<GLuint>
        vertInd,
        textInd,
        normInd;
    std::vector<TextureCoord> tempTextureCoords;

    std::vector<std::vector<GLuint>> tempIndices;
    std::vector<GLuint> fullModelIndices;

    std::vector<Model::ModelPart> parts{};
    std::string line;
    std::string tag;
    MtlLoader mtlLoader;
    int prefV = 0,prefN = 0,prefT = 0;
    while (std::getline(file, line)){
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if(token == "mtllib"){
            std::string mtlName;
            iss >> mtlName;
            mtlLoader.load(path.substr(0,path.find_last_of('/')+1) + mtlName);}
        else if (token == "o") {
            if(!tag.empty()){
                prefV += tempVertices.size();
                prefN += tempNormals.size();
                prefT += tempTextureCoords.size();
                tempIndices.push_back(vertInd);
                parts.emplace_back(tempVertices,
                                   recalculateNormals(tempVertices, fullModelIndices, tempNormals, normInd),
                                   TextureUVMap::recalculateTextureCoords(tempVertices, fullModelIndices,tempTextureCoords,textInd),
                                   tempIndices, tempColors, tag);
                tempTextureCoords.clear();
                tempVertices.clear();
                tempNormals.clear();
                tempColors.clear();
                textInd.clear();
                normInd.clear();
                vertInd.clear();
                tempIndices.clear();
                fullModelIndices.clear();}
            iss >> tag;
        }
        else if (token == "v") {
            Vertex vertex{};
            iss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        } else if (token == "vn") {
            Normal normal{};
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (token == "vt") {
            TextureCoord textureCoord{};
            iss >> textureCoord.u >> textureCoord.v;
            textureCoord.v = 1.0f - textureCoord.v;
            tempTextureCoords.push_back(textureCoord);
        } else if (token == "f") {
            std::string faceToken;
            while (iss >> faceToken) {
                std::vector<std::string> splitter = StringUtils::split(faceToken,"/");
                GLint vertexIndex = std::stoi(splitter[0]) - 1 - prefV;
                fullModelIndices.push_back(vertexIndex);
                vertInd.push_back(vertexIndex);
                textInd.push_back(std::stoi(splitter[1]) - 1 - prefT);
                normInd.push_back(std::stoi(splitter[2]) - 1 - prefN);
            }}
        else if(token == "usemtl"){
            std::string mtltag;
            iss>>mtltag;
            if(!vertInd.empty()){
                tempIndices.push_back(vertInd);
                vertInd.clear();}
            tempColors.push_back(mtlLoader.getMaterialByTag(mtltag));
        }
    }
    file.close();
    std::cout << "I am readed\n";
    tempIndices.push_back(vertInd);
    parts.emplace_back(tempVertices,
                       recalculateNormals(tempVertices, fullModelIndices, tempNormals, normInd),
                       TextureUVMap::recalculateTextureCoords(tempVertices, fullModelIndices, tempTextureCoords,textInd),
                       tempIndices, tempColors, tag);
    return Model{parts};
    }

std::vector<Normal> ObjLoader::recalculateNormals(const std::vector<Vertex>& temp_vertices, const std::vector<GLuint>& vertInd,
                                                  const std::vector<Normal>& temp_normals, const std::vector<GLuint>& normInd) {
    std::vector<Normal>
            vertexNormals(temp_vertices.size(), Normal{0.0f, 0.0f, 0.0f});
    for (size_t i = 0; i < vertInd.size(); i++) {
        vertexNormals[vertInd[i]] += temp_normals[normInd[i]];}
    for (Normal& vertexNormal : vertexNormals) {
        float length = Functionality::scalar2Points(vertexNormal, vertexNormal);
        if (length) {vertexNormal /= length;}}
    return vertexNormals;
}


void ObjLoader::MtlLoader::load(const std::string &path) {
    std::string line;
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open MTL file: " << path << std::endl;
        return;}
    double Ns,d,Ni;
    Color Ka,Kd,Ks,Ke;
    int illum;
    std::string tag;
    Texture texture;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        Switch(token){
            Case("newmtl"):{
                if(!tag.empty()){
                    if(texture.isLoaded()){
                        Material mat{Ns,Ka,texture,Ks,Ke,Ni,d,illum};
                        colors.insert({tag,mat});
                    }else{
                        Material mat{Ns,Ka,Kd,Ks,Ke,Ni,d,illum};
                        colors.insert({tag,mat});}
                    texture = Texture();}
                iss >> tag;
            }break;
            Case("Ns"):{
                iss >> Ns;
            }break;
            Case("Ka"):{
                float r,g,b;
                iss >> r >> g >> b;
                Ka = Color{r,g,b};
            }break;
            Case("Kd"):{
                float r,g,b;
                iss >> r >> g >> b;
                Kd = Color{r,g,b,1};
            }break;
            Case("Ks"):{
                float r,g,b;
                iss >> r >> g >> b;
                Ks = Color{r,g,b};
            }break;
            Case("Ke"):{
                float r,g,b;
                iss >> r >> g >> b;
                Ke = Color{r,g,b};
            }break;
            Case("Ni"):{
                iss >> Ni;
            }break;
            Case("d"):{
                iss >> d;
            }break;
            Case("illum"):{
                iss >> illum;
            }break;
            Case("map_Kd"):{
                std::string subpath = path.substr(0,path.find_last_of('/')+1);
                std::string subpathName;
                iss >> subpathName;
                texture.loadImg(subpath + subpathName);
            }}}
    Material mat;
    if(texture.isLoaded()){mat = {Ns,Ka,texture,Ks,Ke,Ni,d,illum};}
    else{mat = {Ns,Ka,Kd,Ks,Ke,Ni,d,illum};}
    colors.insert({tag,mat});

}

Material ObjLoader::MtlLoader::getMaterialByTag(const std::string& tag) {
    if (colors.contains(tag))
        return colors.at(tag);
    std::cout << "not found:" << tag << "\n";
    return {};
}
