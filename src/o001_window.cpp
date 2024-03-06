#include <glad/glad.h> //@@ glad shoud be included before GLFW
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    //@@ if esc not pressed, glfwGetKet will return GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //@@ set the windowShouldClose as true
        glfwSetWindowShouldClose(window, true);
    }
}

int main() { //@@ init glfw
    std::cout << "gl from 001" << std::endl;
    glfwInit();
    //@@ use glfwWindowHint to setup for glfw window, first paramater is enum, second is int
    //@@ we tell glfw we are using opengl version 3.3 with core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    //@@ for MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    //@@ init glfw window    parameter: width, height, title
    GLFWwindow *window = glfwCreateWindow(800, 600, "@penGL", NULL, NULL);
    //@@ if window is null pointer which means we failed to init the window.
    if (window == NULL) {
        std::cout << "Failed to init GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    //@@ what is context ?
    glfwMakeContextCurrent(window);

    //@@ GLAD is used to manage opengl function pointers
    //@@ so init it before use any opengl function.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!" << std::endl;
        return -1;
    }

    //@@ viewport: render size, usually same with the window size
    //@@ parameter：downleft cornor x, downleft cornor y, render size x, render
    // size y;
    glViewport(0, 0, 800, 600);

    //@@ register "framebuffer_size_callback" as the call back function for window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //@@ we dont want glwindow close after only one frame is rendered, give it a loop.
    //@@ glfwWindowShouldClose(...): if window is asked to close by user
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        //@@ ALL READER CODES SHOULD BE AFTER HERE
        //@@ ====================================
        //@@ clear color buffer at any frame start
        glClear(GL_COLOR_BUFFER_BIT);
        //@@ clear the buffer and fulfil the color buffer use the color below.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //@@ ====================================
        //@@ ALL READER CODES SHOULD BE BEFORE HERE
        glfwSwapBuffers(window);
        //@@ check is any event is triggered, for example keydown, mouseclick etc.
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
