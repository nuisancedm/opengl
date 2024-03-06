#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//@@ look at this, we use a string to write SHADER code. but JUST FOR NOW.
//@@ the string is GLSL language, it is exclusive for opengl shader.
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "} \n";

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    std::cout << "[MESSAGE]: executable launched from file 002" << std::endl;

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
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //@@ a 2D triangle (z is all 0). opengl can only handle with coords in [-1,1]^3, also called "Normalized Device Coordinates\NDC"
    //@@ if data dosnt coord in NDC, it need to be transformed by vertex shader.
    //@@ BTW: below is how coords transformation pipeline in opengl
    //@@ world coords --(vertex shader)--> NDC --(Viewport Transform)--> Screen-space Coords
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    //@@ shader is also opengl object, it needs a unique unsigned int ID.
    unsigned int vertexShader;

    //@@ create vertex shader, return a id
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach the vertexshader source code to vertexShader and compile it.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //@@ really want to know if our shader source code is sucessfully compiled.
    int success;
    char infolog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "[ERROR]:VERTEX_SHADER_COMPILATION_FAILED " << infolog << std::endl;
    }

    //@@ do almost same thing to fragment shader.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //@@ really want to know if our shader source code is sucessfully compiled. (copy paste tbh)
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        std::cout << "[ERROR]:FRAGMENT_SHADER_COMPILATION_FAILED " << infolog << std::endl;
    }

    //@@ new OBJECT! shader program is a final version of shader we gona use, by tie all shader together.
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //@@ really also want to know if you shaders are inked successfully;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
        std::cout << "[ERROR]:SHADER_LINK_FAILED " << infolog << std::endl;
    }

    //@@ after link the progarm, delete the original shader, cuz we have the shader program, shaders are in it.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //@@ first GL object here! which is vertex buffer object.
    //@@ VBO stores tons of vertex data in grapic memory, so shaders could reach this vertexs very fast.
    //@@ opengl objects were distinguished by their unique unsigned int ID.
    //@@ what VAO is? we talk later.
    unsigned int VBO, VAO;

    //@@ talk later.
    glGenVertexArrays(1, &VAO);

    //@@ allocate graphic memories to VBO and return id to VBO
    glGenBuffers(1, &VBO);

    //@@ talk later.
    glBindVertexArray(VAO);

    //@@ VBO's buffer type is GL_ARRAY_BUFFER, so bind it to GL_ARRAY_BUFFER
    //@@ opengl allow to bind multiple buffer object only if they are different type.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //@@ now any call for change GL_ARRAY_BUFFER is call to change VBO
    //@@ the forth parameter in glBufferData() matters:
    //@@ GL_STATIC_DRAW means the buffer data is not gona be changed during the render loop
    //@@ BTW: GL_DYNAMIC_DRAW(changed), GL_STREAM_DRAW (changed every iteration)
    //@@ the reason is that the read/write speed on different graphic memory region is different;  quicker data changes, quicker read/write speed needs.
    //@@ dont forget that glBufferData() is sending vertex data to GL_ARRAY_BUFFER(VBO)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //@@ right now, GPU has a VBO, which is all vertex that he need to draw.
    //@@ GPU have a shader program, which is what color and what position the vertexs need to be.
    //@@ but GPU is silly, he dont know what is really happened in the graphic memory.
    //@@ in GPU's sight of view, what in the graphic memory is just bits and bits and bits and bits.
    //@@ it is necessory to explain to GPU from which bits to which bits is a vertex coords and which bits to which bits is normal of this vertex.
    //@@ and that will be glVertexAttribPointer(position of the attrib, attrib length, attrib data type, Normalize or not, Stride, offset)
    //@@ lets talk VAO now! VAO will record all glVertexAttribPointer() and glEnableVertexAttribArray() and glDisableVertexAttribArray() after we bind VAO (it is smart)
    //@@ so we just need to unbind and rebind VAO when we want to render different set of vertex.
    //@@ we must use VAO in opengl core profile, otherwise crash.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    //@@ enable the 0 vertex attrib.
    glEnableVertexAttribArray(0);

    //@@ unbind VAO and VBO, in the render loop, we only need to bind VAO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //@@ choose the shader program! choose the VAO we want!
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //@@ call the draw! glDrawArrays(primitive, vertex begins, how many vertex you want to draw) 
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
