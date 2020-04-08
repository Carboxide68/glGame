#pragma once

#include "../common/common.h"
#include "node.h"

struct SurfaceGeometry {

    float node1[3];
    float normal[3];
    float node2[3];
    float normal[3];
    float node3[3];
    float normal[3];

};

// TODO:
// Implement ability to relate multiple nodeLists for more versatile meshes 

class Triangle {

public:

    Triangle(std::array<uint, 3> IDs, glm::vec3 normal);

    SurfaceGeometry assembleGeometry(const std::vector<Node> &nodeList);

    ErrWithData<std::array<uint, 3>> getIndices();  //Returns an array of the positions of the nodes in the array, will pick the 
                                                    //first element if node is not found
    ErrWithData<std::array<glm::vec3, 3>> getVertices();//Returns the vertices if they have the right ID, otherwise returns empty
                                                        //vertices
    
    int updateIndices(std::vector<Node> &nodeList); //Updates the indices of the triangle, should be used if the list
                                                    //has been changed

    int containsID(uint ID); //Returns a nullpointer if it doesn't contain that element

private:

    std::array<uint, 3> m_IDs;
    std::array<Node*, 3> m_VectorReferences;
    std::vector<Node> *m_NodeListStart;

    glm::vec3 m_Normal;

};