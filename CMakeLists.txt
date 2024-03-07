cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
 
project(opengl)

include_directories(include)
link_directories(lib)


set(MYGL_SOURCES
    src/glad.c
    src/myGL/Renderer.cpp
    src/myGL/VertexBuffer.cpp
    src/myGL/IndexBuffer.cpp
    src/myGL/VertexArray.cpp
    src/myGL/Shader.cpp

)

#add_executable(opengl src/glad.c src/o001_window.cpp)
#add_executable(opengl src/glad.c src/o002_triangles.cpp)
#add_executable(opengl src/glad.c src/o003_twoTriangles.cpp)
add_executable(opengl  src/o004_abstraction.cpp ${MYGL_SOURCES}) 


target_link_libraries(opengl glfw3) 
