#pragma once

#include "../common/common.h"
#include "node.h"

struct SurfaceGeometry {

    float node1[3];
    float norm1[3];
    float node2[3];
    float norm2[3];
    float node3[3];
    float norm3[3];

};

// TODO:
// Implement ability to relate multiple nodeLists for more versatile meshes 

class Triangle {

public:

    Triangle(std::array<uint, 3> IDs, const std::vector<Node> &nodeList, std::array<glm::vec3, 3> normal);
    Triangle(std::array<uint, 3> IDs, const std::vector<Node> &nodeList);
    Triangle(std::array<uint, 3> IDs);

    SurfaceGeometry assembleGeometry(const std::vector<Node> &nodeList);

    ErrWithData<std::array<uint, 3>> getIndices();  //Returns an array of the positions of the nodes in the array, will pick the 
                                                    //first element if node is not found
    ErrWithData<std::array<glm::vec3, 3>> getVertices();//Returns the vertices if they have the right ID, otherwise returns empty
                                                        //vertices
    
    int updateIndices(std::vector<Node> &nodeList); //Updates the indices of the triangle, should be used if the list
                                                    //has been changed

    int containsID(uint ID); //Returns a nullpointer if it doesn't contain that element
    
    ErrWithData<std::array<glm::vec3, 3>> getVertices();

    void updateVertices(const std::vector<Node> &nodeList);

    void assignNormals(const std::array<glm::vec3, 3> normals);

    void generateNormal(const std::vector<Node> &nodeList);

private:

    std::array<uint, 3> m_IDs;
    std::array<Node*, 3> m_VectorReferences;
    std::vector<Node> *m_NodeListStart;

    std::array<glm::vec3, 3> m_Normal;

};