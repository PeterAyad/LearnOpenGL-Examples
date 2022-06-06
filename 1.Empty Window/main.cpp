#include "iostream"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

// Window resize callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Input Processing Function
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// GLFW custom initialization
void initializeGLFW()
{
    // Initialize GLFW
    glfwInit();
    // Choose OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Choose OpenGL "core profile" mode instead of "immediate mode" which is deprecated
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main()
{
    // 1.Initialize GLFW
    initializeGLFW();

    // 2.Create a window
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGLExample", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 3.Make the window's context current
    glfwMakeContextCurrent(window);

    // 4.Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 5.Set and automate port size
    // Set viewport
    glViewport(0, 0, 800, 600);

    // Set the window's callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 6.Clear buffers
    // Set buffer clearing color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // 7.Render Loop
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Poll events
        glfwPollEvents();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // 8.Clean Up
    // Terminate GLFW
    glfwTerminate();

    return 0;
}