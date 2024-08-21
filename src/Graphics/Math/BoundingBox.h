//
// Created by asuka on 20.08.2024.
//

#ifndef MIRAI_BOUNDINGBOX_H
#define MIRAI_BOUNDINGBOX_H


#include "Structs.h"

class BoundingBox {
private:
    Vertex minusP, plusP;
public:
    BoundingBox() = default;
    BoundingBox(const vertarr& vertexs){
        calc(vertexs);
    }
    void calc(const vertarr& vertexs){
        minusP = vertexs[0];
        plusP = vertexs[0];
        for(int ii1 = 1;ii1 < vertexs.size();ii1++){
            if(vertexs[ii1].x < minusP.x)minusP.x = vertexs[ii1].x;
            if(vertexs[ii1].y < minusP.y)minusP.y = vertexs[ii1].y;
            if(vertexs[ii1].z < minusP.z)minusP.z = vertexs[ii1].z;
            if(vertexs[ii1].x < plusP.x)plusP.x = vertexs[ii1].x;
            if(vertexs[ii1].y < plusP.y)plusP.y = vertexs[ii1].y;
            if(vertexs[ii1].z < plusP.z)plusP.z = vertexs[ii1].z;}
    }
    void updateOneVertex(Vertex vert){
        if(vert.x < minusP.x)minusP.x = vert.x;
        if(vert.y < minusP.y)minusP.y = vert.y;
        if(vert.z < minusP.z)minusP.z = vert.z;
        if(vert.x < plusP.x)plusP.x = vert.x;
        if(vert.y < plusP.y)plusP.y = vert.y;
        if(vert.z < plusP.z)plusP.z = vert.z;
    }
};


#endif //MIRAI_BOUNDINGBOX_H
