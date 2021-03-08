/*
  Copyright Luke A.C.A. Rieff 2020 - All Rights Reserved
*/

#include "main.h"

#define LABEL "[MAIN]: "

int32_t main(int32_t argc, char **argv) {
    printf("Starting....\n");

    if (!glfwInit()) {
        std::cout << LABEL << "Failed to init GLFW!" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << LABEL << "Failed to init GLEW!" << std::endl;
        return -1;
    }

    //
    // Prepares for main loop
    //

    // Reads the shaders ...
    std::cout << LABEL << "Creating Programs/Shaders" << std::endl;
    const Program groundPlaneShader = Program::create("../shaders/ground-plane/fragment.glsl", "../shaders/ground-plane/vertex.glsl");
    std::cout << LABEL << "Programs/Shaders ready!" << std::endl;

    //
    // Starts the main loop
    //

    while (!glfwWindowShouldClose(window)) {
        glClearColor(.2F, .2F, .2F, 1.F);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}