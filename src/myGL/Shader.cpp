#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string filepath) :
    m_Filepath(filepath), m_RendererID(0) {
    ShaderProgramSource source = ParseShader();
    m_RendererID = CreateShader(source.VectexSource, source.FragmentSource);
    glUseProgram(m_RendererID);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader() {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    std::ifstream stream(m_Filepath);
    if (!stream.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + m_Filepath);
    }
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("#SHADER") != std::string::npos) {
            if (line.find("VERTEX") != std::string::npos) {
                // vertex mode
                type = ShaderType::VERTEX;
            } else if (line.find("FRAGMENT") != std::string::npos) {
                // fragment mode
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    //@@ get compile error
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "[ERROR]SHADER_COMPILATION_FAILED: "
                  << " " << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::bind() const {
    GLCall(glUseProgram(m_RendererID));
};
void Shader::unbind() const {
    GLCall(glUseProgram(0));
};

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
};
void Shader::setUniform1i(const std::string &name, int v) {
    GLCall(glUniform1i(getUniformLocation(name), v));
};

void Shader::setUniform1f(const std::string &name, float v) {
    GLCall(glUniform1f(getUniformLocation(name), v));
};

void Shader::setUniformMatrix4f(const std::string &name, const glm::mat4& m) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1,GL_FALSE, glm::value_ptr(m)));
};

unsigned int Shader::getUniformLocation(const std::string &name) {
    GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "[WARNING]uniform " << name << "doesn't exists" << std::endl;
    }
    return location;
};