#include "vbo.h"

VertexBuffer::VertexBuffer() { 
    
    GLCall(glGenBuffers(1, &m_IdVBO));

}