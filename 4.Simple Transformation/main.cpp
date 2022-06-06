#include "iostream"
#include "fstream"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

int main()
{
    GLFWwindow *window = initializeAndCreateWindow();

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, "./shaders/shader.vert");
    GLuint fragShader = createShader(GL_FRAGMENT_SHADER, "./shaders/shader.frag");
    GLuint shaderProgram = createProgram(vertexShader, fragShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.3f, 0.6f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.3f, 0.6f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.6f, 0.0f, 0.3f, 1.0f, 0.0f};

    GLuint indices[] = {
        0, 1, 2,
        0, 1, 3};

    GLuint VAO = createVAO();
    GLuint VBO = createVBO(vertices, sizeof(vertices));
    GLuint EBO = createEBO(indices, sizeof(indices));

    setAttributesPointers_texture();
    setWrappingAndFiltering();

    glActiveTexture(GL_TEXTURE0);
    GLuint texture = createTexture();
    loadTextureImage(false, "./textures/container.jpg");

    glActiveTexture(GL_TEXTURE1);
    GLuint texture2 = createTexture();
    loadTextureImage(true, "./textures/awesomeface.png");

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // $>>>>>>>>> New - start

        // 1. Update vertex shader to accept transformation matrix uniform

        // 2. Prepare transformation matrix
        glm::mat4 transformationMat = glm::mat4(1.0f);
        // transformationMat = glm::translate(transformationMat, glm::vec3(1.0f, 1.0f, 0.0f));
        // transformationMat = glm::rotate(transformationMat, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        transformationMat = glm::rotate(transformationMat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transformationMat = glm::scale(transformationMat, glm::vec3(0.5, 0.5, 0.5));

        // 3. Set transformation matrix uniform
        // ********* DON'T FORGET TO USE THE SHADER PROGRAM FIRST *********
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transformationMat));

        // 4. Clear buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // $>>>>>>>>> New - end

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
