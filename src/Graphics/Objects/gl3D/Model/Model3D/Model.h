#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../BaseModel.h"
#include "../../Camera/PerspectiveCamera.h"
#include "../../../../Shaders/ShaderProgram.h"
#include "../../../Materials/Material.h"
#include "../../../../Buffers/FrameBuffer.h"
#include "../../../gl2D/TextureUVMap.h"
#include "../../../../Buffers/BO.h"
#include <File/ConfigLoader.h>

#ifndef MIRAAI_MODELPART_H
#define MIRAAI_MODELPART_H

class Model : public BaseModel {
private:
    vertarr verticesTexture{
            {1.f, 1.f, 0.0f},
            {1.f, -1.f, 0.0f},
            {-1.f, -1.f, 0.0f},
            {-1.f, 1.f, 0.0f}};
    std::vector<GLfloat> texCoordsTexture{
            1.f, 1.f,
            1.f, 0.f,
            0.f, 0.f,
            0.f, 1.f,};
    std::vector<GLuint> indicesTexture{
            0, 1, 3,
            1, 2, 3};
    ShaderProgram textureShader;
public:
    class ModelPart {
    private:
        vertarr vertices;
        std::vector<Normal> normals;
        vertarr res_vertices;
        std::vector<std::vector<GLuint>> coloredIndices;
        TextureUVMap textureCoords;
        std::vector<Material> color;
        std::string Tag;
        Vertex geom_center{0, 0, 0};
        void renderEnabler();
        void renderDisabler();
    public:
        ModelPart() = default;

        ModelPart(
                const vertarr &vertices,
                const std::vector<Normal> &normals,
                TextureUVMap textureCoords,
                const std::vector<std::vector<GLuint>> &indices,
                const std::vector<Material> &color,
                std::string Tag = "");

        void setVertex(const vertarr &data);

        vertarr &getVertex();

        void setNormals(const std::vector<Normal> &data);

        std::vector<Normal> &getNormals();

        void setIndices(const std::vector<std::vector<GLuint>> &data);

        std::vector<std::vector<GLuint>> &getIndices();

        void setColors(const std::vector<Material> &data);

        std::vector<Material> &getColors();

        void setTextureCoords(const TextureUVMap &data);

        TextureUVMap &getTextureCoords();

        std::string &getTag();

        void setTag(const std::string &Tag);
        GLuint getOffset();
        Vertex &getGeomCenter();

        void render(ShaderProgram program);

        void reset();

        void destroy();
    };

private:
    std::vector<ModelPart> parts;
    s2i taggedParts{};
    FrameBuffer fbuff;
    ShaderProgram
            modelShader,
            outlineShader;
    BO buffers{};


    glm::mat4 renderSetupMatrix();

    void renderSetupShader(const glm::mat4 &modelMatrix, PerspectiveCamera cam);

    void renderFinalFrame();

    void renderMain(const glm::mat4 &modelMatrix, PerspectiveCamera cam);

    void standardCode();

    void bindBuffers();

public:
    void destroy() override;

    Model(const std::vector<ModelPart> &parts);

    Model();

    ~Model();

    std::vector<Model::ModelPart> &getParts();

    void render() override;

    virtual void render(PerspectiveCamera cam);

    Model::ModelPart &getModelPartByTag(const std::string &tag);
};

#endif // MIRAAI_MODELPART_H