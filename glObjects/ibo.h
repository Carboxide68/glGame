#include "../common/common.h"
class IndexBuffer {

public:

    IndexBuffer();

    inline void bufferData(unsigned int size, void *data) const {use(); GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));}

    inline void bufferData(unsigned int size) const {bufferData(size, NULL);}

    inline void extendData(unsigned int size, size_t offset, void *data) const {use(); GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, size, offset, data));}

    inline void use() const {GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IdIBO));}

private:

    unsigned int m_IdIBO;

};