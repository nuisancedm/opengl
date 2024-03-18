#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
#include "Renderer.h"
#include <iostream>

Texture::Texture(const std::string filepath, textureWrapper wrapper, textureFilter filter, bool y_flip) :
    m_Filepath(filepath), m_RendererID(0), m_Wrapper(wrapper), m_Filter(filter), m_Yflip(y_flip) {
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    parseTexture();
}

Texture::~Texture() {
    unbind();
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::parseTexture() {
    if (m_Yflip) {
        stbi_set_flip_vertically_on_load(true);
    }
    unsigned char *data = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (data) {
        switch (m_Wrapper) {
        case REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case MIRRORED_REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
            // case CLAMP_TO_BORDER:
            //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            //     break;
        }

        switch (m_Filter) {
        case LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        throw std::runtime_error("Failed to open texture file: " + m_Filepath);
    }
    stbi_image_free(data);
};

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}