#include "BO.h"
#include <stdexcept>

// VBO Implementation

BO::VBO::VBO() {}

BO::VBO::~VBO() {
    if (!oVBO.empty()) {
        glDeleteBuffers(static_cast<GLsizei>(oVBO.size()), oVBO.data());
    }
}

void BO::VBO::generate(int count) {
    oVBO.resize(count);
    glGenBuffers(count, oVBO.data());
}

void BO::VBO::bind(int index) const {
    if (index < 0 || index >= static_cast<int>(oVBO.size())) {
        throw std::out_of_range("Invalid VBO index");
    }
    glBindBuffer(GL_ARRAY_BUFFER, oVBO[index]);
}

void BO::VBO::setData(const std::vector<Vertex>& data) const {
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
}

void BO::VBO::setSubData(GLuint id, int& offset, const std::vector<Vertex>& vertices) const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(Vertex), vertices.data());
    offset+=vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const std::vector<GLuint>& BO::VBO::getOvbo() const {
    return oVBO;
}

BO::VAO::VAO() {

}

BO::VAO::~VAO() {
    if (!oVAO.empty()) {
        glDeleteVertexArrays(static_cast<GLsizei>(oVAO.size()), oVAO.data());
    }
}

void BO::VAO::generate(int count) {
    oVAO.resize(count);
    glGenVertexArrays(count, oVAO.data());
}

void BO::VAO::bind(int index) const {
    if (index < 0 || index >= static_cast<int>(oVAO.size())) {
        throw std::out_of_range("Invalid VAO index");
    }
    glBindVertexArray(oVAO[index]);
}

void BO::VAO::bindData(int index) const {
    if (index < 0 || index >= static_cast<int>(oVAO.size())) {
        throw std::out_of_range("Invalid VAO index");
    }
    glBindVertexArray(oVAO[index]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
}

const std::vector<GLuint>& BO::VAO::getOvao() const{
    return oVAO;
}
BO::BO() {

}

void BO::genVBO(int count) {
    vbo.generate(count);
}

void BO::genVAO(int count) {
    vao.generate(count);
}

BO::VBO& BO::getVbo() {
    return vbo;
}

BO::VAO& BO::getVao() {
    return vao;
}
