#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
//@@ opengl abstraction
#include "myGL/myGL.h"
//@@ imGUI
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_glfw.h"
#include "imGUI/imgui_impl_opengl3.h"
//@@ glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "global.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightpos = glm::vec3(1.0f, 1.0f, 1.0f);

float BoxVertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "@pengl", NULL, NULL);

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
    std::cout << "[MESSAGE]: executable launched from file 010" << std::endl;

    //@@ print the max number of textures the graphic card support.
    int maxTextureUnits;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << maxTextureUnits << std::endl;

    GLCall(glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT));

    //@@ hide the mouse and capture it. like the FPS model
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    {
        VertexArray va_cube;
        VertexArray va_light;

        VertexBuffer vb(BoxVertices, sizeof(BoxVertices));

        Shader shader("../src/shader/o013.shader");
        Shader light_shader("../src/shader/light.shader");

        Texture texture1("../src/texture/container2.png", REPEAT, LINEAR, true);
        Texture texture2("../src/texture/container2_specular.png", REPEAT, LINEAR, true);

        VertexBufferLayout layout;
        layout.Push(GL_FLOAT, 3); // 0 position
        layout.Push(GL_FLOAT, 3); // 1 normal
        layout.Push(GL_FLOAT, 2); // 1 normal

        va_cube.addBuffer(vb, layout);

        VertexBufferLayout layout_light;
        layout_light.Push(GL_FLOAT, 3); // 0 position
        layout_light.Push(GL_FLOAT, 3); // 1 normal
        layout_light.Push(GL_FLOAT, 2); // 1 normal



        va_light.addBuffer(vb, layout_light);

        vb.unbind();

        shader.unbind();
        light_shader.unbind();

        va_light.unbind();
        va_cube.unbind();

        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        float lightX = 1.2f;
        float lightY = 1.0f;
        float lightZ = 2.0f;
        bool isRollingBox = true;

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);
            renderer.clear();

            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.fov), float(SRC_WIDTH) / float(SRC_HEIGHT), 0.1f, 100.0f);
            glm::mat4 model_light = glm::mat4(1.0f);
            glm::mat4 model_box = glm::mat4(1.0f);

            //@@ draw box
            shader.bind();
            shader.setUniform3f("lightPos", float(lightpos.x), float(lightpos.y), float(lightpos.z));
            shader.setUniform3f("viewPos", float(camera.Position.x), float(camera.Position.y), float(camera.Position.z));
            shader.setUniformMatrix4f("view", view);
            shader.setUniformMatrix4f("projection", projection);

            shader.setUniform1i("material.diffuse", 0);
            shader.setUniform1i("material.specular", 1);
            shader.setUniform1f("material.shininess", 32.0f);

            shader.setUniform3f("light.ambient",  0.2f, 0.2f, 0.2f);
            shader.setUniform3f("light.diffuse",  0.5f, 0.5f, 0.5f);
            shader.setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

            if (isRollingBox) {
                model_box = glm::rotate(model_box, (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));
                shader.setUniformMatrix4f("model", model_box);
            } else {
                shader.setUniformMatrix4f("model", model_box);
            }
            va_cube.bind();
            texture1.bind(0);
            texture2.bind(1);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //@@ draw light
            light_shader.bind();
            light_shader.setUniformMatrix4f("view", view);
            light_shader.setUniformMatrix4f("projection", projection);
            model_light = glm::translate(model_light, glm::vec3(float(lightpos.x), float(lightpos.y), float(lightpos.z)));
            model_light = glm::scale(model_light, glm::vec3(0.1f)); // a smaller cube
            light_shader.setUniformMatrix4f("model", model_light);
            va_light.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //@@ ============= IMGUI ====================
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Control Pannel");                                                        // Create a window called "Hello, world!" and append into it.
                ImGui::Text("average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate); //@@ fps
                ImGui::Text("none");                                                                   // Display some text (you can use a format strings too)
                ImGui::SliderFloat("lightX", &lightpos.x, -5.0f, 5.0f);
                ImGui::SliderFloat("lightY", &lightpos.y, -5.0f, 5.0f);
                ImGui::SliderFloat("lightZ", &lightpos.z, -5.0f, 5.0f);
                ImGui::SliderFloat("fov", &camera.fov, 1.0f, 179.0f);
                ImGui::Checkbox("Roll", &isRollingBox); 
                
                ImGui::End();
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    //@@ Esc to close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    //@@ WASD to the camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}