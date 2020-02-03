#include "mesh.h"

Mesh::Mesh() : meshShader("shader/meshShader.vertexShader", "shader/meshShader.fragmentShader") {

    m_ModelMatrix = glm::mat4(1);
    m_rGen.seed(floor(glfwGetTime() * 100000));
    m_MeshLoaded = false;

}

void Mesh::addVertex(glm::vec3 vertex) {
    
    // Node node = {m_rGen(), vertex};
    m_Vertices.push_back(vertex);

}
// void Mesh::addVertex(glm::vec3 vertex) {
    
//     m_Vertices.push_back(Node(m_rGen(), vertex));

// }

void Mesh::addVertices(std::vector<glm::vec3> vertices) {
    for (auto i = vertices.begin(); i != vertices.end(); i++) {
        m_Vertices.push_back(*i);
    }
}

// void Mesh::addVertices(std::vector<glm::vec3> vertices) {
//     for (auto i = vertices.begin(); i != vertices.end(); i++) {
//         m_Vertices.push_back(Node(m_rGen(), (*i)));
//     }
// }

void Mesh::createTriangle(std::array<ushort, 3> indices) {

    std::array<ushort, 3> IDs;

    // for (int i = 0; i < indices.size(); i++) {
    //     IDs[i] = m_Vertices[i].getId();
    // }

    m_Surfaces.push_back(indices);
}

void Mesh::createTriangles(std::vector<std::array<ushort, 3>> triangles) {
    for (auto i = triangles.begin(); i != triangles.end(); i++) {
        createTriangle(*i);
    }
}

// void Mesh::assembleModelTexture() {

//     m_ModelTextures = {};
//     {
//     std::vector<float*> tempdata;
//     for (auto i = m_Instances.begin(); i != m_Instances.end(); i++) {
//         m_ModelTextures.size += 4;
//         tempdata.push_back((float*)value_ptr(*i));
//     }
//     m_ModelTextures.data = (void *)tempdata.data();
//     }
// }

void Mesh::loadMesh() {
    
    ushort indexRaw[m_Surfaces.size() * 3];
    float vertexRaw[m_Vertices.size() * 3];
    uint indexSize = sizeof(uint) * 3 * m_Surfaces.size();
    uint vertexSize = sizeof(float) * 3 * m_Vertices.size();
    for (uint i = 0; i < m_Surfaces.size(); i++) {
        indexRaw[i * 3] = m_Surfaces[i][0];
        indexRaw[i * 3 + 1] = m_Surfaces[i][1];
        indexRaw[i * 3 + 2] = m_Surfaces[i][2];
    }

    for (uint i = 0; i < m_Vertices.size(); i++) {
        vertexRaw[i * 3] = m_Vertices[i][0];
        vertexRaw[i * 3 + 1] = m_Vertices[i][1];
        vertexRaw[i * 3 + 2] = m_Vertices[i][2];
    }
    m_VAO.bufferData(vertexSize, static_cast<void*>(vertexRaw));
    m_VAO.bufferIndices(indexSize, static_cast<void*>(indexRaw));

    m_VAO.addAttrib(GL_FLOAT, 3);

    m_MeshLoaded = true;
}

void Mesh::draw(const Camera &camera) {
    if (m_MeshLoaded) {
        glm::mat4 assembledMatrix = camera.getPerspectiveMatrix() * camera.getViewMatrix() * m_ModelMatrix;
        meshShader.use();
        meshShader.setUniform("assembledMatrix", assembledMatrix);
        meshShader.setUniform("model", m_ModelMatrix);
        m_VAO.drawIndices(m_Surfaces.size() * 3);
    }
}

void Mesh::temp_setPosition(glm::vec3 pos) {
    m_ModelMatrix[3][0] = pos.x;
    m_ModelMatrix[3][1] = pos.y;
    m_ModelMatrix[3][2] = pos.z;
}

void Mesh::temp_scale(float scale) {
    temp_scalev = scale;
    m_ModelMatrix[0][0] *= scale;
    m_ModelMatrix[0][1] *= scale;
    m_ModelMatrix[0][2] *= scale;
    m_ModelMatrix[1][0] *= scale;
    m_ModelMatrix[1][1] *= scale;
    m_ModelMatrix[1][2] *= scale;
    m_ModelMatrix[2][0] *= scale;
    m_ModelMatrix[2][1] *= scale;
    m_ModelMatrix[2][2] *= scale;
}

void Mesh::temp_rotateX(float angle) {

    glm::mat4 rotationMatrix = glm::mat4(
        1,  0,          0,          0,
        0,  cos(angle), sin(angle), 0,
        0, -sin(angle), cos(angle), 0,
        0,  0,          0,          1
    );

    glm::mat4 tempMat = rotationMatrix * m_ModelMatrix;
    m_ModelMatrix[0][0] = tempMat[0][0];
    m_ModelMatrix[0][1] = tempMat[0][1];
    m_ModelMatrix[0][2] = tempMat[0][2];
    m_ModelMatrix[1][0] = tempMat[1][0];
    m_ModelMatrix[1][1] = tempMat[1][1];
    m_ModelMatrix[1][2] = tempMat[1][2];
    m_ModelMatrix[2][0] = tempMat[2][0];
    m_ModelMatrix[2][1] = tempMat[2][1];
    m_ModelMatrix[2][2] = tempMat[2][2];
}

void Mesh::temp_rotateY(float angle) {
    glm::mat4 rotationMatrix = glm::mat4(
        cos(angle), 0, -sin(angle), 0,
        0,          1,  0,          0,
        sin(angle), 0,  cos(angle), 0,
        0,          0,  0,          1
    );
    m_ModelMatrix = rotationMatrix * m_ModelMatrix;
}
    
void Mesh::parseFile(std::string file) {

    std::string myFile = loadFile(file);

    std::vector<std::string> lines;

    std::string temp;
    for (auto i = myFile.begin(); i != myFile.end(); i++) {
        if (*i != '\n') {
            temp += *i;
        } else {
            lines.push_back(temp);
            temp = "";
        }
    }

    for (auto i = lines.begin(); i != lines.end(); i++) {
        std::string line = *i;
        if (line[0] == 'v') {
            if (line[1] == ' ') {
                parseVertex(line);
            }
        } else if (line[0] == 'f') {
            parseFace(line);
        }   
    }
}

void Mesh::parseVertex(std::string line) {
    float vertices[3];
    sscanf(line.c_str(), "%*s %f %f %f %*s", &vertices[0], &vertices[1], &vertices[2]);
    // printf("%f %f %f\n", vertices[0], vertices[1], vertices[2]);
    addVertex(glm::vec3(vertices[0], vertices[1], vertices[2]));
}

void Mesh::parseFace(std::string line) {
    ushort indices[7];
    uint count = sscanf(line.c_str(), "%*s %hu %*s %hu %*s %hu %*s %hu %*s %hu %*s %hu %*s %hu %*s", &indices[0], &indices[1], &indices[2], &indices[3], &indices[4], &indices[5], &indices[6]);
    // printf("%hu %hu %hu\n", indices[0], indices[1], indices[2]);
    if (count == 3) {
        createTriangle({indices[0] - 1, indices[1] - 1, indices[2] - 1});
    } else if (count == 4) {
        createTriangle({indices[0] - 1, indices[1] - 1, indices[2] - 1});
        createTriangle({indices[0] - 1, indices[3] - 1, indices[2] - 1});
    }
}

void Mesh::temp_resetModel() {
    glm::mat3 tempMat = glm::mat3(temp_scalev);
    m_ModelMatrix[0][0] = tempMat[0][0];
    m_ModelMatrix[0][1] = tempMat[0][1];
    m_ModelMatrix[0][2] = tempMat[0][2];
    m_ModelMatrix[1][0] = tempMat[1][0];
    m_ModelMatrix[1][1] = tempMat[1][1];
    m_ModelMatrix[1][2] = tempMat[1][2];
    m_ModelMatrix[2][0] = tempMat[2][0];
    m_ModelMatrix[2][1] = tempMat[2][1];
    m_ModelMatrix[2][2] = tempMat[2][2];
}