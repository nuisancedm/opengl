cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
 
project(opengl)

include_directories(include src/vender)
link_directories(lib)


set(MYGL_SOURCES
    src/glad.c
    src/myGL/Renderer.cpp
    src/myGL/VertexBuffer.cpp
    src/myGL/IndexBuffer.cpp
    src/myGL/VertexArray.cpp
    src/myGL/Shader.cpp
    src/myGL/Texture.cpp
)

set(IMGUI_SOURCES
    src/vender/imGUI/imgui.cpp    
    src/vender/imGUI/imgui_demo.cpp
    src/vender/imGUI/imgui_draw.cpp
    src/vender/imGUI/imgui_tables.cpp
    src/vender/imGUI/imgui_widgets.cpp 
    src/vender/imGUI/imgui_impl_glfw.cpp
    src/vender/imGUI/imgui_impl_opengl3.cpp
)

#add_executable(opengl src/glad.c src/o001_window.cpp)
#add_executable(opengl src/glad.c src/o002_triangles.cpp)
#add_executable(opengl src/glad.c src/o003_twoTriangles.cpp)
#add_executable(opengl  src/o004_abstraction.cpp ${MYGL_SOURCES}) 
#add_executable(opengl  src/o005_texture.cpp ${MYGL_SOURCES})  
add_executable(opengl  src/o006_imgui.cpp ${MYGL_SOURCES} ${IMGUI_SOURCES})  


target_link_libraries(opengl glfw3) 
