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

        // 1. To start drawing in 3D we'll first create a model matrix.
        // The model matrix consists of translations, scaling and/or rotations we'd like to apply to transform all object's vertices to the global world space
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // 2.Next we need to create a view matrix. We want to move slightly backwards in the scene so the object becomes visible (when in world space we're located at the origin (0,0,0)).
        // note that we're translating the scene in the reverse direction of where we want to move
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // 3.The last thing we need to define is the projection matrix. We want to use perspective projection for our scene
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // 4. Update vertex shader to accept model, view, projection uniforms

        // 5. Set the matrices uniforms
        // ********* DON'T FORGET TO USE THE SHADER PROGRAM FIRST *********
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // $>>>>>>>>> New - end

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
