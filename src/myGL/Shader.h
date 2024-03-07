#pragma once
#include <string>

struct ShaderProgramSource {
    std::string VectexSource;
    std::string FragmentSource;
};

class Shader {
private:
    std::string m_Filepath;
    unsigned int m_RendererID;
    //@@ caching for unifrom
public:
    Shader(const std::string filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    //@@ set uniform
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

private:
    unsigned int getUniformLocation(const std::string &name);
    ShaderProgramSource ParseShader();
    unsigned int CompileShader(unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
};