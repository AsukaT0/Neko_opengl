//
// Created by asuka on 29.05.2024.
//

#ifndef MIRAI_TEXTUREUVMAP_H
#define MIRAI_TEXTUREUVMAP_H


#include <vector>
#include "../../Math/Structs.h"

class TextureUVMap {
private:
    std::vector<TextureCoord>
        interCoords,defaultCoords;

public:
    TextureUVMap(std::vector<TextureCoord> textureCoords);
    TextureUVMap();;
    std::vector<TextureCoord>&getCoords();
    TextureCoord* getTextureCoordsLink();
    void setBias(float u,float v);;
    static TextureUVMap recalculateTextureCoords(
            const std::vector<Vertex>& temp_vertices,const  std::vector<GLuint>& vertInd,
            const std::vector<TextureCoord>& tempTextureCoords,const  std::vector<GLuint>& textInd);
};


#endif //MIRAI_TEXTUREUVMAP_H
