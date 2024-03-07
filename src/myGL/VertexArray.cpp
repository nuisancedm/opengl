#include "Renderer.h"
#include "VertexArray.h"


VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_RendererID));
    GLCall(glBindVertexArray(m_RendererID));
}
VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}
void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    bind();
    vb.bind();
    unsigned int offset = 0;
    const auto &elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void *)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}