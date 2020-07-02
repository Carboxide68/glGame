#include "vao.h"

VertexArray::VertexArray() {

    m_IndexType = GL_UNSIGNED_SHORT;

    GLCall(glGenVertexArrays(1, &m_IdVAO));
    use();
    VBO.use();
    IBO.use();
    disuse();
}

void VertexArray::addAttrib(GLenum type, unsigned int count) {
    
    use();

    m_ArrayLayout.push_back(VertexArrayLayout{type, count});

    unsigned int stride = 0;
    unsigned int offset = 0;

    for (auto i = m_ArrayLayout.begin(); i != m_ArrayLayout.end(); i++) {
        stride += (*i).count * getSizeOfType((*i).type);
    }
    
    for (unsigned int i = 0; i < m_ArrayLayout.size(); i++) {

        GLCall(glEnableVertexAttribArray(i));
        
        GLCall(glVertexAttribPointer(i, m_ArrayLayout[i].count, m_ArrayLayout[i].type, GL_FALSE, stride, (void *)(intptr_t)offset));

        offset += m_ArrayLayout[i].count * getSizeOfType(m_ArrayLayout[i].type);

    }

    disuse();

}

unsigned int VertexArray::getSizeOfType(GLenum type) const {
    switch (type) {
        case GL_FLOAT:
            return sizeof(float);

        case GL_INT:
            return sizeof(int);
        
        case GL_UNSIGNED_INT:
            return sizeof(uint);
        
        default:
            printf("Type not supported! %d", type);
            return 0;
    }
}

void VertexArray::destroy() {
    GLCall(glDeleteVertexArrays(1, &m_IdVAO));
}