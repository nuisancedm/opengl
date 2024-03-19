#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "myGL/Renderer.h"
#include "myGL/IndexBuffer.h"
#include "myGL/VertexBuffer.h"
#include "myGL/VertexBufferLayout.h"
#include "myGL/VertexArray.h"
#include "myGL/Shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
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
    GLFWwindow *window = glfwCreateWindow(800, 600, "@penGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to init GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "[MESSAGE]: executable launched from file 003" << std::endl;

    GLCall(glViewport(0, 0, 800, 600));
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        // first
        0.6f, 0.5f, 0.0f,
        0.6f, -0.5f, 0.0f,
        -0.4f, 0.5f, 0.0f,
        // second
        0.4f, -0.5f, 0.0f,
        -0.6f, -0.5f, 0.0f,
        -0.6f, 0.5f, 0.0f};

    unsigned int indice[] = {
        //@@ start from 0!
        0, 1, 2,
        3, 4, 5};

    {   
        //@@ it is a basic example of how to use the abstractions.
        //@@ VAO, VBO, IBO and SHADER are abstracted.
        //@@ always va first
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indice, 6);

        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 3);
        va.addBuffer(vb, layout);

        Shader shader("../src/shader/basic.shader");
        shader.bind();

        float r = 0.0f;
        float increment = 0.05f;

        vb.unbind();
        //@@ remember we dont unbind index buffer.
        va.unbind();

        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            //@@ uniform is gone to be seted per draw.
            shader.setUniform4f("u_Color", r, 0.5f, 0.5f, 1.0f);
            if (r > 1.0f) {
                increment = -0.05f;
            } else if (r < 0.0f) {
                increment = 0.05f;
            }
            r += increment;

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
