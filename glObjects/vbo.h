#pragma once

#include "../common/common.h"

class VertexBuffer {

public:

    VertexBuffer();

    inline void bufferData(unsigned int size, void *data) const {use(); GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));}

    inline void bufferData(unsigned int size) const {bufferData(size, NULL);}

    inline void extendData(unsigned int size, size_t offset, void *data) const {use(); GLCall(glBufferSubData(GL_ARRAY_BUFFER, size, offset, data));}

    inline void use() const {GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_IdVBO));}

private:

    unsigned int m_IdVBO;

};