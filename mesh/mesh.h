#pragma once

#include "../common/common.h"
#include "../glObjects/vao.h"
#include "../glObjects/vbo.h"
#include "../shader/shader.h"
#include "../camera/camera.h"
#include "node.h"
#include "triangle.h"
#include <random>

/* 
    This class makes a triangle based mesh.
    !Add more documentation!
*/

class Mesh {

public:

    Mesh();

    void addVertex(glm::vec3 vertex);

    void addVertices(std::vector<glm::vec3> vertices);

    void createTriangle(std::array<uint, 3> indices, glm::vec3 normal);

    void createTriangles(std::vector<std::array<uint, 3>> triangles, std::vector<glm::vec3> normals);

    void createInstance(glm::mat4 modelMatrix);

    void loadMesh();

    void draw(const Camera &camera);

    void temp_setPosition(glm::vec3 positions);
    void temp_scale(float scale);
    void temp_rotateX(float angle);
    void temp_rotateY(float angle);
    void temp_rotateZ(float angle);
    void temp_resetModel();
    
    void parseFile(std::string file);

    Shader meshShader;

private:

    // struct VoidWithSize {
    //     void *data;
    //     uint size;
    // };

    // VoidWithSize m_ModelTextures;

    VertexArray m_VAO;
    
    glm::mat4 m_ModelMatrix;
  
    // std::vector<Node> m_Vertices;
    std::vector<Node> m_Vertices;
    std::vector<Triangle> m_Surfaces;

    // std::vector<glm::mat4> m_Instances;
    // uint m_TextureId;

    std::mt19937 m_rGen;

    // void assembleModelTexture();

    // void setupTexture();

    bool m_MeshLoaded;
    float temp_scalev = 1.0f;

    void parseVertex(std::string line);
    void parseFace(std::string line);

};