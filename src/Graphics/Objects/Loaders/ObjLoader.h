
#include "../gl3D/Model/Model3D/Model.h"
#include "../Materials/Material.h"

class ObjLoader{
    private:
    static std::vector<Normal> recalculateNormals(
            const std::vector<Vertex>& temp_vertices, const std::vector<GLuint>& vertInd,
            const std::vector<Normal>& temp_normals, const std::vector<GLuint>& normInd);
    static std::vector<std::string> lines;
    class MtlLoader{
    private:
        std::map<std::string,Material> colors;
    public:
        void load(const std::string& path);
        Material getMaterialByTag(const std::string& tag);};
    public:
    static Model load(const std::string& path);

};