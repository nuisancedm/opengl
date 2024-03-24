#pragma once
#include <string>

enum textureWrapper {
    REPEAT,MIRRORED_REPEAT,CLAMP_TO_EDGE,CLAMP_TO_BORDER,
};
enum textureFilter{
    NEAREST,LINEAR
};

class Texture{
private:
    unsigned int m_RendererID;
    std::string m_Filepath;
    

public:
    int m_Width;
    int m_Height;
    int m_Channels;
    textureWrapper m_Wrapper;
    textureFilter m_Filter;
    bool m_Yflip;

    Texture(const std::string filepath,textureWrapper wraper, textureFilter filter, const bool y_flip);
    ~Texture();


    void parseTexture();
    std::string getFileExtension(const std::string& filepath);
    void loadPNG();
    void loadJPEG();
    void setTextureParameters();
    void bind(unsigned int slot =0) const;
    void unbind() const;
};