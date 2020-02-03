
#include "ibo.h"

IndexBuffer::IndexBuffer() {

    GLCall(glGenBuffers(1, &m_IdIBO));   

}