cmake_minimum_required(VERSION 3.27)
project(OpenGl)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenGl main.cpp)

target_link_libraries(OpenGl opengl32)

include_directories(${OPENGL_INCLUDE_DIRS})

