cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
 
project(opengl)

include_directories(include)
link_directories(lib)

add_executable(opengl src/glad.c src/main.cpp)
target_link_libraries(opengl glfw3) 
