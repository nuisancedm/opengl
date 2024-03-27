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

glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
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
    std::cout << "[MESSAGE]: executable launched from file 014" << std::endl;

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

        Shader shader("../src/shader/o014.shader");
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
        float light_color[4][4] = {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};

        while (!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            processInput(window);
            renderer.clear();

            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.fov), float(SRC_WIDTH) / float(SRC_HEIGHT), 0.1f, 100.0f);
            
            

            //@@ draw box
            shader.bind();
            shader.setUniform3f("viewPos", float(camera.Position.x), float(camera.Position.y), float(camera.Position.z));
            shader.setUniformMatrix4f("view", view);
            shader.setUniformMatrix4f("projection", projection);

            shader.setUniform1i("material.diffuse", 0);
            shader.setUniform1i("material.specular", 1);
            shader.setUniform1f("material.shininess", 32.0f);

            shader.setUniform3f("dirLight.direction",  -0.2f, -1.0f, -0.3f);
            shader.setUniform3f("dirLight.ambient",  0.2f, 0.2f, 0.2f);
            shader.setUniform3f("dirLight.diffuse",  0.5f, 0.5f, 0.5f);
            shader.setUniform3f("dirLight.specular", 1.0f, 1.0f, 1.0f);

            shader.setUniform3f("pointLights[0].position", float(pointLightPositions[0].x),float(pointLightPositions[0].y),float(pointLightPositions[0].z));
            shader.setUniform3f("pointLights[1].position", float(pointLightPositions[1].x),float(pointLightPositions[1].y),float(pointLightPositions[1].z));
            shader.setUniform3f("pointLights[2].position", float(pointLightPositions[2].x),float(pointLightPositions[2].y),float(pointLightPositions[2].z));
            shader.setUniform3f("pointLights[3].position", float(pointLightPositions[3].x),float(pointLightPositions[3].y),float(pointLightPositions[3].z));

            shader.setUniform3f("pointLights[0].ambient", 0.2f * light_color[0][0], 0.2f * light_color[0][1], 0.2f * light_color[0][2]);
            shader.setUniform3f("pointLights[1].ambient", 0.2f * light_color[1][0], 0.2f * light_color[1][1], 0.2f * light_color[1][2]);
            shader.setUniform3f("pointLights[2].ambient", 0.2f * light_color[2][0], 0.2f * light_color[2][1], 0.2f * light_color[2][2]);
            shader.setUniform3f("pointLights[3].ambient", 0.2f * light_color[3][0], 0.2f * light_color[3][1], 0.2f * light_color[3][2]);

            shader.setUniform3f("pointLights[0].diffuse", 0.5f * light_color[0][0], 0.5f * light_color[0][1], 0.5f * light_color[0][2]);
            shader.setUniform3f("pointLights[1].diffuse", 0.5f * light_color[1][0], 0.5f * light_color[1][1], 0.5f * light_color[1][2]);
            shader.setUniform3f("pointLights[2].diffuse", 0.5f * light_color[2][0], 0.5f * light_color[2][1], 0.5f * light_color[2][2]);
            shader.setUniform3f("pointLights[3].diffuse", 0.5f * light_color[3][0], 0.5f * light_color[3][1], 0.5f * light_color[3][2]);

            shader.setUniform3f("pointLights[0].specular", 1.0f * light_color[0][0], 1.0f * light_color[0][1], 1.0f * light_color[0][2]);
            shader.setUniform3f("pointLights[1].specular", 1.0f * light_color[1][0], 1.0f * light_color[1][1], 1.0f * light_color[1][2]);
            shader.setUniform3f("pointLights[2].specular", 1.0f * light_color[2][0], 1.0f * light_color[2][1], 1.0f * light_color[2][2]);
            shader.setUniform3f("pointLights[3].specular", 1.0f * light_color[3][0], 1.0f * light_color[3][1], 1.0f * light_color[3][2]);

            shader.setUniform1f("pointLights[0].constant",1.0f);
            shader.setUniform1f("pointLights[1].constant",1.0f);
            shader.setUniform1f("pointLights[2].constant",1.0f);
            shader.setUniform1f("pointLights[3].constant",1.0f);

            shader.setUniform1f("pointLights[0].linear",0.09f);
            shader.setUniform1f("pointLights[1].linear",0.09f);
            shader.setUniform1f("pointLights[2].linear",0.09f);
            shader.setUniform1f("pointLights[3].linear",0.09f);

            shader.setUniform1f("pointLights[0].quadratic",0.032f);
            shader.setUniform1f("pointLights[1].quadratic",0.032f);
            shader.setUniform1f("pointLights[2].quadratic",0.032f);
            shader.setUniform1f("pointLights[3].quadratic",0.032f);

            shader.setUniform3f("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
            shader.setUniform3f("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
            shader.setUniform1f("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
            shader.setUniform3f("spotLight.ambient", 0.2f, 0.2f, 0.2f);
            shader.setUniform3f("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
            shader.setUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);


            va_cube.bind();
            texture1.bind(0);
            texture2.bind(1);

            for (int i = 0; i < 10; i++)
            {
                glm::mat4 model_box = glm::mat4(1.0f);
                
                model_box = glm::translate(model_box, cubePositions[i]);
                float angle = 20.0f * (i + 1);
                //model_box = glm::rotate(model_box, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

                if (isRollingBox)
                {
                    model_box = glm::rotate(model_box, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
                    shader.setUniformMatrix4f("model", model_box);
                }
                else
                {
                    shader.setUniformMatrix4f("model", model_box);
                }

                glDrawArrays(GL_TRIANGLES, 0, 36);

            }
            

            //@@ draw light
            light_shader.bind();
            va_light.bind();
            light_shader.setUniformMatrix4f("view", view);
            light_shader.setUniformMatrix4f("projection", projection);
            for (int i = 0; i < 4; i++)
            {   
                light_shader.setUniform3f("lightColor", float(light_color[i][0]),float(light_color[i][1]),float(light_color[i][2]));
                glm::mat4 model_light = glm::mat4(1.0f);
                model_light = glm::translate(model_light, glm::vec3(float(pointLightPositions[i].x), float(pointLightPositions[i].y), float(pointLightPositions[i].z)));
                model_light = glm::scale(model_light, glm::vec3(0.1f)); // a smaller cube
                light_shader.setUniformMatrix4f("model", model_light);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            //@@ ============= IMGUI ====================
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Control Pannel");                                                        // Create a window called "Hello, world!" and append into it.
                ImGui::Text("average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate); //@@ fps                                                              // Display some text (you can use a format strings too)
                
                ImGui::Checkbox("Roll", &isRollingBox);

                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) // 默认展开
                {
                    ImGui::SliderFloat("fov", &camera.fov, 1.0f, 179.0f);
                }

                if (ImGui::CollapsingHeader("Light_0"))
                {
                    ImGui::SliderFloat("X_light0", &pointLightPositions[0].x, -5.0f, 5.0f);
                    ImGui::SliderFloat("Y_light0", &pointLightPositions[0].y, -5.0f, 5.0f);
                    ImGui::SliderFloat("Z_light0", &pointLightPositions[0].z, -5.0f, 5.0f);
                    ImGui::ColorEdit4("Color_light0", light_color[0]);
                }
                if (ImGui::CollapsingHeader("Light_1"))
                {
                    ImGui::SliderFloat("X_light1", &pointLightPositions[1].x, -5.0f, 5.0f);
                    ImGui::SliderFloat("Y_light1", &pointLightPositions[1].y, -5.0f, 5.0f);
                    ImGui::SliderFloat("Z_light1", &pointLightPositions[1].z, -5.0f, 5.0f);
                    ImGui::ColorEdit4("Color_light1", light_color[1]);
                }
                if (ImGui::CollapsingHeader("Light_2"))
                {
                    ImGui::SliderFloat("X_light2", &pointLightPositions[2].x, -5.0f, 5.0f);
                    ImGui::SliderFloat("Y_light2", &pointLightPositions[2].y, -5.0f, 5.0f);
                    ImGui::SliderFloat("Z_light2", &pointLightPositions[2].z, -5.0f, 5.0f);
                    ImGui::ColorEdit4("Color_light2", light_color[2]);
                }
                if (ImGui::CollapsingHeader("Light_3"))
                {
                    ImGui::SliderFloat("X_light3", &pointLightPositions[3].x, -5.0f, 5.0f);
                    ImGui::SliderFloat("Y_light3", &pointLightPositions[3].y, -5.0f, 5.0f);
                    ImGui::SliderFloat("Z_light3", &pointLightPositions[3].z, -5.0f, 5.0f);
                    ImGui::ColorEdit4("Color_light3", light_color[3]);
                }
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
        glfwSetScrollCallback(window, nullptr);
        firstMouse = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetScrollCallback(window, scroll_callback);
        
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