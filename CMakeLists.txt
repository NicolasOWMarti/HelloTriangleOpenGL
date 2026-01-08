cmake_minimum_required(VERSION 3.10)
project(HelloTriangleOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# macOS OpenGL framework
find_library(OPENGL_LIBRARY OpenGL)

# Homebrew include/library paths
set(HOME_BREW_PREFIX "/opt/homebrew") # Change to /usr/local on Intel

# Include directories
include_directories(
    ${HOME_BREW_PREFIX}/include
    ${HOME_BREW_PREFIX}/include/GL
    ${CMAKE_SOURCE_DIR}/src
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
)

# Libraries
set(LIBS
    ${HOME_BREW_PREFIX}/lib/libglfw.3.dylib
    ${HOME_BREW_PREFIX}/lib/libGLEW.2.2.dylib
    ${OPENGL_LIBRARY}
)

# Add executable
add_executable(OpenGLApp src/main.cpp)

# Link libraries
target_link_libraries(OpenGLApp ${LIBS})
