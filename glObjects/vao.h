#pragma once
#include <vector>

#include "../common/common.h"
#include "vbo.h"

struct VertexArrayLayout {

    GLenum type;
    unsigned int count;

};

class VertexArray {

public:

    VertexArray();

    void addAttrib(GLenum type, unsigned int count);

    inline void use() const {GLCall(glBindVertexArray(m_IdVAO));}
    inline void disuse() const {GLCall(glBindVertexArray(0));}

    inline void bufferData(unsigned int size, void *data) const {use(); VBO.bufferData(size, data); disuse();}
    inline void bufferData(unsigned int size) const {use(); VBO.bufferData(size); disuse();}
    inline void extendData(unsigned int size, size_t offset, void *data) const {use(); VBO.extendData(size, offset, data); disuse();}

    inline void drawArrays(unsigned int count, unsigned int offset) const {use(); GLCall(glDrawArrays(GL_TRIANGLES, offset, count)); disuse();}
    inline void drawArrays(unsigned int count) const {use(); GLCall(glDrawArrays(GL_TRIANGLES, 0, count)); disuse();}

    void destroy();

private:

    VertexBuffer VBO;

    unsigned int m_IdVAO;
    std::vector<VertexArrayLayout> m_ArrayLayout;

    unsigned int getSizeOfType(GLenum type) const;
    
};