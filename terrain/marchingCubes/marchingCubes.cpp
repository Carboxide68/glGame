#include "marchingCubes.h"
#include "marchingcubesdata.h"
#include <chrono>

#define ABOVE(x, y) ((x > y) ? 1 : 0)

#define SCPU STANDARD_CHUNK_POINT_UNIT

MeshGenerator::MeshGenerator(uint seed, uint chunkSize) : m_Noise(-32, 32, seed) {

    m_ChunkSize = chunkSize;


    STANDARD_CHUNK_POINT_UNIT = STANDARD_CHUNK_UNIT/(chunkSize - 1);

}

std::vector<std::array<glm::vec3, 3>> MeshGenerator::GetChunk(glm::vec3 chunkPos) {

    for (int i = 0; i < (const int)m_StoredChunks.size(); i++) {
        if (m_StoredChunks[i] == chunkPos) {
            return m_Chunks[i];
        }
    }
    
    m_StoredChunks.push_back(chunkPos);
    std::vector<std::array<glm::vec3, 3>> chunk;
    chunk = generateChunk(chunkPos);
    m_Chunks.push_back(chunk);

    return chunk;
    
}

std::vector<std::array<glm::vec3, 3>> MeshGenerator::generateChunk(glm::vec3 chunkPos) {
    chunkPos *= STANDARD_CHUNK_UNIT;
    std::vector<uint> cubes;
    std::vector<std::vector<std::vector<float>>> pointMap(m_ChunkSize, std::vector<std::vector<float>>(m_ChunkSize, std::vector<float>(m_ChunkSize)));
    std::vector<std::array<glm::vec3, 3>> chunkGeometry;
    for (int x = 0; x < (const int)m_ChunkSize; x++) {
        for (int y = 0; y < (const int)m_ChunkSize; y++) {
            for (int z = 0; z < (const int)m_ChunkSize; z++) {
                pointMap[x][y][z] = m_Noise.Noise3D({
                                                    static_cast<float>(x) * STANDARD_CHUNK_POINT_UNIT + chunkPos.x, 
                                                    static_cast<float>(y) * STANDARD_CHUNK_POINT_UNIT + chunkPos.y, 
                                                    static_cast<float>(z) * STANDARD_CHUNK_POINT_UNIT + chunkPos.z
                                                    });
            }
        }
    }
    std::vector<std::array<glm::vec4, 8>> cubeCorners;
    for (int x = 0; x < (const int)m_ChunkSize - 1; x++) {
        for (int y = 0; y < (const int)m_ChunkSize - 1; y++) {
            for (int z = 0; z < (const int)m_ChunkSize - 1; z++) {
                cubes.push_back(
                    (
                    ABOVE(pointMap[x  ][y  ][z  ], threshHold)  * 1     +
                    ABOVE(pointMap[x+1][y  ][z  ], threshHold)  * 2     +
                    ABOVE(pointMap[x+1][y  ][z+1], threshHold)  * 4     +
                    ABOVE(pointMap[x  ][y  ][z+1], threshHold)  * 8     +
                    ABOVE(pointMap[x  ][y+1][z  ], threshHold)  * 16    +
                    ABOVE(pointMap[x+1][y+1][z  ], threshHold)  * 32    +
                    ABOVE(pointMap[x+1][y+1][z+1], threshHold)  * 64    +
                    ABOVE(pointMap[x  ][y+1][z+1], threshHold)  * 128
                    ));
                cubeCorners.push_back({
                    glm::vec4((x  ) * SCPU, (y  )   * SCPU, (z  )   * SCPU, pointMap[x  ][y  ][z  ]),
                    glm::vec4((x+1) * SCPU, (y  )   * SCPU, (z  )   * SCPU, pointMap[x+1][y  ][z  ]),
                    glm::vec4((x+1) * SCPU, (y  )   * SCPU, (z+1)   * SCPU, pointMap[x+1][y  ][z+1]),
                    glm::vec4((x  ) * SCPU, (y  )   * SCPU, (z+1)   * SCPU, pointMap[x  ][y  ][z+1]),
                    glm::vec4((x  ) * SCPU, (y+1)   * SCPU, (z  )   * SCPU, pointMap[x  ][y+1][z  ]),
                    glm::vec4((x+1) * SCPU, (y+1)   * SCPU, (z  )   * SCPU, pointMap[x+1][y+1][z  ]),
                    glm::vec4((x+1) * SCPU, (y+1)   * SCPU, (z+1)   * SCPU, pointMap[x+1][y+1][z+1]),
                    glm::vec4((x  ) * SCPU, (y+1)   * SCPU, (z+1)   * SCPU, pointMap[x  ][y+1][z+1])
                });
            }
        }
    }
    for (int i = 0; i < (const int)((m_ChunkSize - 1) * (m_ChunkSize - 1) * (m_ChunkSize - 1)); i++) {
        std::array<glm::vec3, 3> tri;
        for (int a = 0; triangulation[cubes[i]][a] != -1; a += 3) {
            int a0 = cornerIndexAFromEdge[triangulation[cubes[i]][a]];
            int b0 = cornerIndexBFromEdge[triangulation[cubes[i]][a]];

            int a1 = cornerIndexAFromEdge[triangulation[cubes[i]][a+1]];
            int b1 = cornerIndexBFromEdge[triangulation[cubes[i]][a+1]];

            int a2 = cornerIndexAFromEdge[triangulation[cubes[i]][a+2]];
            int b2 = cornerIndexBFromEdge[triangulation[cubes[i]][a+2]];

            tri[0] = interpolateVerts(cubeCorners[i][a0], cubeCorners[i][b0]);
            tri[1] = interpolateVerts(cubeCorners[i][a1], cubeCorners[i][b1]);
            tri[2] = interpolateVerts(cubeCorners[i][a2], cubeCorners[i][b2]);
            chunkGeometry.push_back(tri);
        }
    }
    return chunkGeometry;
}

glm::vec3 MeshGenerator::interpolateVerts(glm::vec4 v1, glm::vec4 v2) {
    float t = (threshHold - v1.w)/(v2.w - v1.w);
    return glm::vec3(v1) + t * (glm::vec3(v2) - glm::vec3(v1)); 
}

MarchingCubes::MarchingCubes() : m_MeshGenerator(std::chrono::high_resolution_clock::now().time_since_epoch().count(), 32), 
                                 m_Shader("shader/standard.vert", "shader/standard.frag") {

    m_Model = std::make_shared<Model>(Model());

}

void MarchingCubes::LoadChunk(glm::vec3 chunkPos) {

    std::vector<std::array<glm::vec3, 3>> chunkData = m_MeshGenerator.GetChunk(chunkPos);
    std::vector<glm::vec3> vertices(chunkData.size() * 3);
    std::vector<std::vector<uint>> indices(chunkData.size());
    for (int i = 0;  i < (const int)chunkData.size(); i++) {
        for (int x = 0; x < 3; x++) {
            vertices[i * 3 + x] = chunkData[i][x] + chunkPos;
        }
        indices[i] = {(uint)i * 3 + 0, (uint)i * 3 + 1, (uint)i * 3 + 2};
    }
    Mesh& chunkMesh = m_Model->createMesh();
    char name[30];
    sprintf(name, "Chunk %.0f %.0f %.0f", chunkPos.x, chunkPos.y, chunkPos.z);
    chunkMesh.Name = std::string(name);
    chunkMesh.Vertices = vertices;
    chunkMesh.createPolygons(indices, std::vector<std::vector<uint>>(), std::vector<std::vector<glm::vec3>>());
    chunkMesh.addGroup("marchingGroup");
    m_Model->loadToBuffer();
}

void MarchingCubes::LoadChunks(const std::vector<glm::vec3>& chunks) {
    const size_t chunkSize = chunks.size();
    for (int k = 0; k < (const int)chunkSize; k++) {
        std::vector<std::array<glm::vec3, 3>> chunkData = m_MeshGenerator.GetChunk(chunks[k]);
        std::vector<glm::vec3> vertices(chunkData.size() * 3);
        std::vector<std::vector<uint>> indices(chunkData.size());
        for (int i = 0;  i < (const int)chunkData.size(); i++) {
            for (int x = 0; x < 3; x++) {
                vertices[i * 3 + x] = chunkData[i][x] + chunks[k];
            }
            indices[i] = {(uint)i * 3 + 0, (uint)i * 3 + 1, (uint)i * 3 + 2};
        }
        Mesh& chunkMesh = m_Model->createMesh();
        char name[30];
        sprintf(name, "Chunk %.0f %.0f %.0f", chunks[k].x, chunks[k].y, chunks[k].z);
        chunkMesh.Name = std::string(name);
        chunkMesh.Vertices = vertices;
        chunkMesh.createPolygons(indices, std::vector<std::vector<uint>>(), std::vector<std::vector<glm::vec3>>());
    }
    m_Model->loadToBuffer();
}

void MarchingCubes::Draw(Camera camera, Shader *shader) {
    if (shader == nullptr) {
        shader = &m_Shader;
    }
    shader->use();
    m_Model->draw(shader);
}