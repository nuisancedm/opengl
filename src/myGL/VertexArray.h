#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
    unsigned int m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void bind() const {
        GLCall(glBindVertexArray(m_RendererID));
    }
    void unbind() const {
        GLCall(glBindVertexArray(0));
    }
};