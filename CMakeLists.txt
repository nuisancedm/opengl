cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
 
project(opengl)

include_directories(include src/vender)
link_directories(lib)

# 使用GLOB收集文件，但要注意，这不会自动感知新文件的添加
file(GLOB MYGL_SOURCES "src/myGL/*.cpp" "src/glad.c")
file(GLOB IMGUI_SOURCES "src/vender/imGUI/*.cpp")

# 创建两个静态库：MYGL和IMGUI
add_library(MYGL STATIC ${MYGL_SOURCES})
add_library(IMGUI STATIC ${IMGUI_SOURCES})

# 为MYGL和IMGUI设置包含目录
target_include_directories(MYGL PUBLIC "include" "src/vender")
target_include_directories(IMGUI PUBLIC "include" "src/vender")

#add_executable(opengl src/o001_window.cpp)
#add_executable(opengl src/o002_triangles.cpp)
#add_executable(opengl src/o003_twoTriangles.cpp)
#add_executable(opengl  src/o004_abstraction.cpp) 
#add_executable(opengl  src/o005_texture.cpp)  
#add_executable(opengl  src/o006_imgui.cpp)  
#add_executable(opengl  src/o007_transformation.cpp)  
#add_executable(opengl  src/o008_coordinates.cpp)
add_executable(opengl  src/o009_camera.cpp)


target_link_libraries(opengl MYGL IMGUI glfw3)
