#include <File/ConfigLoader.h>
#include "Model.h"
#include <algorithm>
#include <iostream>
#include <File/File.h>
#include <thread>
#include <numeric>
#include <utility>

#include "../../../../../Controller.h"
#include "../../../../Math/Functionality.h"


Model::~Model() {
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    modelShader.destroy();
//    outlineShader.destroy();
}

Model::Model() {

}

Model::Model(const std::vector<ModelPart> &parts) : parts(parts) {
    for (int ii1 = 0; ii1 < parts.size(); ii1++) {
        taggedParts.emplace((this->parts)[ii1].getTag(), ii1);
    }
    standardCode();

    buffers.genVAO(2);
    buffers.genVBO(2);
    fbuff = FrameBuffer(Controller::mainWindows->getHeight(), Controller::mainWindows->getHeight());
}

void Model::render(PerspectiveCamera cam) {
    bindBuffers();
    glPushMatrix();
    glLoadIdentity();

    fbuff.begin();
    renderMain(renderSetupMatrix(), cam);
    fbuff.end();

    glPopMatrix();

    renderFinalFrame();
}

glm::mat4 Model::renderSetupMatrix() {
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f),
                                           glm::vec3(modelPosXGlobal, modelPosYGlobal, modelPosZGlobal));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(modelScaleXLocal, modelScaleYLocal, modelScaleZLocal));
    modelMatrix = glm::rotate(modelMatrix, modelRotXGlobal, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, modelRotYGlobal, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, modelRotZGlobal, glm::vec3(0.0f, 0.0f, 1.0f));

    if (modelRotKX != 0 || modelRotKY != 0 || modelRotKZ != 0) {
        modelMatrix = glm::rotate(modelMatrix, modelDegree, glm::vec3(modelRotKX, modelRotKY, modelRotKZ));
    }
    return modelMatrix;
}

void Model::renderSetupShader(const glm::mat4 &modelMatrix, PerspectiveCamera cam) {
    GLuint modelMatrixLoc = modelShader.getUniPath("modelMatrix");
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLuint camMatrixLoc = modelShader.getUniPath("camMatrix");
    glUniformMatrix4fv(camMatrixLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix() * cam.getViewMatrix()));

    GLuint lightPosLoc = modelShader.getUniPath("lightPos");
    glUniform3f(lightPosLoc, 30.0f, 100.0f, 30.0f);

    GLuint lightColLoc = modelShader.getUniPath("lightColor");
    glUniform3f(lightColLoc, 1.0f, 1.0f, 1.0f);

    GLuint camPosLoc = modelShader.getUniPath("CamPos");
    glUniform3f(camPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
}

void Model::renderMain(const glm::mat4 &modelMatrix, PerspectiveCamera cam) {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1, 1, 1);

    buffers.getVao().bind(0);
    buffers.getVbo().bind(0);
    int offsetFin = 0;
    for (ModelPart i: parts) {
        buffers.getVbo().setSubData(0, offsetFin, i.getVertex());
    }
    modelShader.begin();
    {
        renderSetupShader(modelMatrix, cam);
        for (ModelPart &part: parts) {
            glActiveTexture(GL_TEXTURE0);
            part.render(modelShader);
            glDisable(GL_TEXTURE_2D);
        }
    }
    modelShader.end();
    glDisable(GL_DEPTH_TEST);
}

void Model::renderFinalFrame() {
    glPushMatrix();
    glLoadIdentity();
    buffers.getVao().bind(1);
    buffers.getVbo().bind(1);
    int offset = 0;
    buffers.getVbo().setSubData(1, offset, verticesTexture);
    textureShader.begin();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, verticesTexture.data());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, texCoordsTexture.data());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbuff.getTexture().getTextureLink());
    glUniform1i(textureShader.getUniPath("tex"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbuff.getDepthTexture());
    glUniform1i(textureShader.getUniPath("depthTexture"), 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fbuff.getNormalTexture());
    glUniform1i(textureShader.getUniPath("normalTexture"), 0);
    glUniform2f(textureShader.getUniPath("texelSize"), 1.0f / (float) Controller::mainWindows->getWidth(),
                1.0f / (float) Controller::mainWindows->getHeight());
    glUniform1f(textureShader.getUniPath("outlineWidth"), 3.0f);
    glUniform4f(textureShader.getUniPath("outlineColor"), 0.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indicesTexture.data());

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    textureShader.end();
    glPopMatrix();
}

void Model::standardCode() {
    modelShader = ShaderProgram("assets/shaders/vertex/default_models.vert",
                                "assets/shaders/fragment/default_models.frag");

    textureShader = ShaderProgram("assets/shaders/textures/vertex/default_texture.vert",
                                  "assets/shaders/textures/fragment/default_texture.frag");

    modelShader.setupShader();
    textureShader.setupShader();
}

std::vector<Model::ModelPart> &Model::getParts() {
    return parts;
}

Model::ModelPart &Model::getModelPartByTag(const std::string &tag) {
    return parts[taggedParts[tag]];
}

void Model::render() {
    BaseModel::render();
}


Model::ModelPart::ModelPart(const vertarr &vertices, const std::vector<Normal> &normals,
                            TextureUVMap textureCoords, const std::vector<std::vector<GLuint>> &indices,
                            const std::vector<Material> &color, std::string Tag)
        : vertices(vertices), textureCoords(std::move(textureCoords)), coloredIndices(indices), normals(normals),
          Tag(std::move(Tag)), color(color) {
    std::vector<GLuint> mergedVector;
    mergedVector.reserve(std::accumulate(indices.begin(), indices.end(), 0u,
                                         [](size_t sum, const auto &v) { return sum + v.size(); }));
    for (auto v: vertices) {
        res_vertices.push_back(v);
    }
    for (const auto &i: indices) {
        mergedVector.insert(mergedVector.end(), i.begin(), i.end());
    }
    std::thread([&] {
        Vertex vert{0, 0, 0};
        for (const auto &v: vertices) {
            vert += v;
        }
        if (!vertices.empty()) {
            vert /= static_cast<float>(vertices.size());
        }
        geom_center = vert;
    }).join();
}

void Model::ModelPart::setVertex(const vertarr &data) { vertices = data; }

vertarr &Model::ModelPart::getVertex() { return vertices; }

void Model::ModelPart::setNormals(const std::vector<Normal> &data) { normals = data; }

std::vector<Normal> &Model::ModelPart::getNormals() { return normals; }

void Model::ModelPart::setIndices(const std::vector<std::vector<GLuint>> &data) { coloredIndices = data; }

std::vector<std::vector<GLuint>> &Model::ModelPart::getIndices() { return coloredIndices; }

void Model::ModelPart::setTextureCoords(const TextureUVMap &data) { textureCoords = data; }

TextureUVMap &Model::ModelPart::getTextureCoords() { return textureCoords; }

void Model::ModelPart::setColors(const std::vector<Material> &data) { color = data; }

std::vector<Material> &Model::ModelPart::getColors() { return color; }

std::string &Model::ModelPart::getTag() { return Tag; }

void Model::ModelPart::setTag(const std::string &Tag) { this->Tag = Tag; }

void Model::ModelPart::render(ShaderProgram program) {
    renderEnabler();
    GLuint fragColLoc = program.getUniPath("FragColor");
    for (size_t ii1 = 0; ii1 < coloredIndices.size(); ii1++) {
        glm::vec3 fragCol = (ii1 < color.size()) ? glm::vec3(color[ii1].getKdColor().r, color[ii1].getKdColor().g,
                                                             color[ii1].getKdColor().b) : glm::vec3(1.0f);
        glUniform1i(program.getUniPath("useTexture"), color[ii1].getKdTexture().isLoaded());
        if (color[ii1].getKdTexture().isLoaded()) {
            glBindTexture(GL_TEXTURE_2D, color[ii1].getKdTexture().getTextureLink());
            glUniform1i(program.getUniPath("textureSampler"), 0);
        }
        glUniform3fv(fragColLoc, 1, &fragCol[0]);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(coloredIndices[ii1].size()), GL_UNSIGNED_INT,
                       coloredIndices[ii1].data());
    }
    renderDisabler();
}

void Model::ModelPart::renderEnabler() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices.data());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, normals.data());
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, textureCoords.getTextureCoordsLink());
}

void Model::ModelPart::renderDisabler() {
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

GLuint Model::ModelPart::getOffset() {
    return (sizeof vertices);
}


Vertex &Model::ModelPart::getGeomCenter() {
    return geom_center;
}

void Model::ModelPart::destroy() {
    color.clear();
}

void Model::ModelPart::reset() {
    std::copy(res_vertices.begin(), res_vertices.end(), vertices.begin());
}

void Model::destroy() {
    fbuff.destroy();
    modelShader.destroy();
    outlineShader.destroy();
    parts.clear();
    BaseModel::destroy();
}

void Model::bindBuffers() {
}
