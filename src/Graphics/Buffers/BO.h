//
// Created by asuka on 25.06.2024.
//

#ifndef MIRAI_BO_H
#define MIRAI_BO_H


#include <GL/glew.h>
#include <vector>
#include <mutex>
#include "../Math/Structs.h"

class BO {
public:
    class VBO {
        std::vector<GLuint> oVBO;
    public:
        VBO();
        ~VBO();
        void generate(int count);
        void bind(int index) const;
        void setData(const std::vector<Vertex>& data) const;
        void setSubData(GLuint id, int& offset, const std::vector<Vertex>& vertices) const;
        const std::vector<GLuint>& getOvbo() const;
    };

    class VAO {
        std::vector<GLuint> oVAO;
    public:
        VAO();
        ~VAO();
        void generate(int count);
        void bind(int index) const;
        void bindData(int index) const;
        const std::vector<GLuint>& getOvao() const;
    };

private:
    VBO vbo;
    VAO vao;

public:
    BO();
    void genVBO(int count);
    void genVAO(int count);
    VBO& getVbo();
    VAO& getVao();
};
#endif //MIRAI_BO_H
