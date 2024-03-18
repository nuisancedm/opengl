#pragma once
#include <string>

struct ShaderProgramSource {
    std::string VectexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_Filepath;
    
    //@@ caching for unifrom
public:
    unsigned int m_RendererID;
    Shader(const std::string filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    //@@ set uniform
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string &name, int v);
    void setUniform1f(const std::string &name, float v);

private:
    unsigned int getUniformLocation(const std::string &name);
    ShaderProgramSource ParseShader();
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
};