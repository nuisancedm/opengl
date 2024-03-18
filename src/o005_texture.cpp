#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "myGL/Renderer.h"
#include "myGL/IndexBuffer.h"
#include "myGL/VertexBuffer.h"
#include "myGL/VertexArray.h"
#include "myGL/Shader.h"
#include "myGL/Texture.h"

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_glfw.h"

//@@ new file! to add images as a texture.
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

float mixValue = 0.4f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(800, 600, "@pengl", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to init GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "[MESSAGE]: executable launched from file 005" << std::endl;

    //@@ print the max number of textures the graphic card support.
    int maxTextureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << maxTextureUnits << std::endl;

    GLCall(glViewport(0, 0, 800, 600));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f   // 左上
    };

    unsigned int indice[] = {
        0, 1, 3,
        1, 2, 3};

    {
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indice, 6);

        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 3); //@@ 0 position
        layout.Push(GL_FLOAT, 3); //@@ 1 color
        layout.Push(GL_FLOAT, 2); //@@ 2 text coords
        va.addBuffer(vb, layout);

        //@@ codes below were abstracted
        // int width, height, nrChannels;
        // unsigned char *data = stbi_load("../src/texture/container.jpg", &width, &height, &nrChannels, 0);
        // unsigned int texture;
        // glGenTextures(1, &texture);
        // glBindTexture(GL_TEXTURE_2D, texture);

        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // if (data) {
        //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //     glGenerateMipmap(GL_TEXTURE_2D);
        // } else {
        //     std::cout << "Failed to load texture" << std::endl;
        // }

        // stbi_image_free(data);

        Texture texture1("../src/texture/container.jpg", REPEAT, LINEAR, false);
        Texture texture2("../src/texture/hachiware.jpg", REPEAT, LINEAR, true);

        Shader shader("../src/shader/o005.shader");

        shader.setUniform1i("texture0", 0);
        shader.setUniform1i("texture1", 1);

        

        vb.unbind();
        va.unbind();

        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            glActiveTexture(GL_TEXTURE0);
            texture1.bind();

            glActiveTexture(GL_TEXTURE1);
            texture2.bind();

            shader.setUniform1f("mixrate", mixValue);

            va.bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
            va.unbind();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();

    return 0;
}