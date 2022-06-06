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

// $>>>>>>>>> New - start

glm::mat4 rotatingCamera()
{
    // camera/view transformation
    glm::mat4 view = glm::mat4(1.0f);
    float radius = 10.0f;
    float camX = static_cast<float>(sin(glfwGetTime()) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
    view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),  // camera position
                       glm::vec3(0.0f, 0.0f, 0.0f),  // Where we are looking at
                       glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector
    return view;
}

bool wKeyPressed = false;
bool sKeyPressed = false;
bool aKeyPressed = false;
bool dKeyPressed = false;

void processInput_walkAroundCamera(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        wKeyPressed = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        sKeyPressed = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        aKeyPressed = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        dKeyPressed = true;
}

glm::mat4 walkAroundCamera()
{
    static glm::mat4 view = glm::mat4(1.0f);
    static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Calculate frame loading time to have a smooth camera movement (constant speed on all devices)
    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 2.5f * deltaTime;

    if (wKeyPressed == true)
    {
        wKeyPressed = false;
        cameraPos += cameraSpeed * cameraFront;
    }
    if (sKeyPressed == true)
    {
        sKeyPressed = false;
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (aKeyPressed == true)
    {
        aKeyPressed = false;
        // Note that we normalize the resulting right vector. If we wouldn't normalize this vector, the resulting cross product may return differently sized vectors based on the cameraFront variable. If we would not normalize the vector we would move slow or fast based on the camera's orientation instead of at a consistent movement speed.
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (dKeyPressed == true)
    {
        dKeyPressed = false;
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    return view;
}

// $>>>>>>>>> New - end

void setMVP(GLuint shaderProgram)
{
    // $>>>>>>>>> New - start
    glm::mat4 view;
    // view = rotatingCamera();
    // or
    view = walkAroundCamera();
    // $>>>>>>>>> New - end

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

int main()
{
    std::cout << "Use W A S D to move camera!" << std::endl; 
    GLFWwindow *window = initializeAndCreateWindow();

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, "./shaders/shader.vert");
    GLuint fragShader = createShader(GL_FRAGMENT_SHADER, "./shaders/shader.frag");
    GLuint shaderProgram = createProgram(vertexShader, fragShader);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    GLuint VAO = createVAO();
    GLuint VBO = createVBO(vertices, sizeof(vertices));
    setAttributesPointers_cube();

    glEnable(GL_DEPTH_TEST);

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

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    while (!glfwWindowShouldClose(window))
    {
        processInput_walkAroundCamera(window);

        setMVP(shaderProgram);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}
