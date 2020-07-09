#pragma once

#include "perlinNoise.h"
#include "../../mesh/model.h"
#include "../../camera/camera.h"
#include <type_traits>

class MeshGenerator {
public:

    explicit MeshGenerator(uint seed, uint size);

    std::vector<std::array<glm::vec3, 3>> GetChunk(glm::vec3 chunkPos);

    int threshHold = 0;

private:

    std::vector<std::array<glm::vec3, 3>> generateChunk(glm::vec3 chunkPos);

    glm::vec3 interpolateVerts(glm::vec4 v1, glm::vec4 v2);

    PerlinNoise m_Noise;

    uint m_ChunkSize;
    std::vector<glm::vec3> m_StoredChunks;
    std::vector<std::vector<std::array<glm::vec3, 3>>> m_Chunks;

    const float STANDARD_CHUNK_UNIT = 1.0f;
    float STANDARD_CHUNK_POINT_UNIT;

};

class MarchingCubes {

public:

    MarchingCubes();

    void LoadChunk(glm::vec3 chunkPos);
    void LoadChunks(const std::vector<glm::vec3>& chunkPos);

    void Draw(Camera camera, Shader *shader = nullptr);

    inline std::shared_ptr<Model> GetModelReference() {return m_Model;}

private:

    MeshGenerator m_MeshGenerator;

    std::shared_ptr<Model> m_Model;
    Shader m_Shader;

    size_t m_DrawCount;

};