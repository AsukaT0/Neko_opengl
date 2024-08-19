//
// Created by asuka on 29.05.2024.
//

#include "TextureUVMap.h"

TextureCoord *TextureUVMap::getTextureCoordsLink() {
    return interCoords.data();}

std::vector<TextureCoord> &TextureUVMap::getCoords() {
    return interCoords;}

TextureUVMap::TextureUVMap(std::vector<TextureCoord> textureCoords) {
    interCoords = textureCoords;
    defaultCoords = textureCoords;
}

TextureUVMap
TextureUVMap::recalculateTextureCoords(const std::vector<Vertex> &temp_vertices, const std::vector<GLuint> &vertInd,
                                       const std::vector<TextureCoord> &tempTextureCoords,
                                       const std::vector<GLuint> &textInd) {
    std::vector<TextureCoord> ret(temp_vertices.size(), {0,0});
    for(size_t ii1= 0;ii1 < temp_vertices.size();ii1++){
        ret[vertInd[ii1]] = tempTextureCoords[textInd[ii1]];}
    return {ret};
}

void TextureUVMap::setBias(float u, float v) {
    for(size_t ii1 = 0;ii1 < defaultCoords.size();ii1++){
        TextureCoord newCoord{defaultCoords[ii1].u+u,defaultCoords[ii1].v+v};
        interCoords[ii1] = newCoord;}}


TextureUVMap::TextureUVMap() = default;
