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
#include "myGL/Texture.h"

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_glfw.h"
#include "imGUI/imgui_impl_opengl3.h"

//@@ new file! to add images as a texture.
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

float mixValue = 0.4f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(800, 600, "@pengl", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to init GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "[MESSAGE]: executable launched from file 006" << std::endl;

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

        Texture texture1("../src/texture/container.jpg", REPEAT, LINEAR, false);
        Texture texture2("../src/texture/hachiware.jpg", REPEAT, LINEAR, true);

        Shader shader("../src/shader/o005.shader");

        shader.setUniform1i("texture0", 0);
        shader.setUniform1i("texture1", 1);

        vb.unbind();
        va.unbind();
        ib.unbind();
        shader.unbind();

        Renderer renderer;

        //@@ ======== imgui magic code start ============
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        //@@ ======== imgui magic code end ==============

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);
            //@@ ======== imgui magic code start ============
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Hello, world!");                                                                     // Create a window called "Hello, world!" and append into it.
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate); //@@ fps
                ImGui::Text("change the mixValue");                                                                // Display some text (you can use a format strings too)
                ImGui::SliderFloat("mixValue", &mixValue, 0.0f, 1.0f);                                             // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::End();
            }
            ImGui::Render();
            //@@ ======== imgui magic code end ==============

            renderer.clear();

            shader.bind();
            shader.setUniform1f("mixrate", mixValue);

            texture1.bind(0);
            texture2.bind(1);

            renderer.draw(va, ib, shader);

            //@@ ======== imgui magic code start ============
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            //@@ ======== imgui magic code end ==============

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    //@@ ======== imgui magic code start ============
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    //@@ ======== imgui magic code end ==============

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}