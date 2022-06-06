#include "iostream"
#include "fstream"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initializeGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// This function initializes libraries and creates a window
GLFWwindow *initializeAndCreateWindow()
{
    initializeGLFW();
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGLExample", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return window;
}

// $>>>>>>>>> New
// This function takes file path of shader GLSL and set it as shader's source
void sourceShader(unsigned int shader, const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
}

// This create shaders
GLuint createShader(GLenum type, const std::string &filePath)
{
    // Define a variable to carry the ID
    unsigned int shader;
    // Create shader ID
    shader = glCreateShader(type);
    // Add source code to shader
    sourceShader(shader, filePath);
    // compile the shader
    glCompileShader(shader);
    // return the shader ID
    return shader;
}

// This function creates a program and attach shaders to it
GLuint createProgram(GLuint vertexShader, GLuint fragShader)
{
    // Define a variable to carry the ID
    unsigned int shaderProgram;
    // Create program ID
    shaderProgram = glCreateProgram();
    // Link the program
    glUseProgram(shaderProgram);

    // Add shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    // Link (create pipeline)
    glLinkProgram(shaderProgram);

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    // return the program ID
    return shaderProgram;
}

// This function creates VAO
GLuint createVAO()
{
    // Define a variable to carry the ID
    unsigned int vao;
    // Create VAO ID
    glGenVertexArrays(1, &vao);
    // Bind VAO
    glBindVertexArray(vao);
    // return the VAO ID
    return vao;
}

// This function creates VBO
GLuint createVBO(float *vertices, unsigned int size)
{
    // Define a variable to carry the ID
    unsigned int vbo;
    // Create VBO ID
    glGenBuffers(1, &vbo);
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Add data to VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    // return the VBO ID
    return vbo;
}

// This function creates EBO
GLuint createEBO(unsigned int *indices, unsigned int size)
{
    // Define a variable to carry the ID
    unsigned int ebo;
    // Create EBO ID
    glGenBuffers(1, &ebo);
    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Add data to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    // return the EBO ID
    return ebo;
}

// This function set attributes pointers
void setAttributesPointers()
{
    // Set our vertex attributes pointers
    // COORDINATES == O
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // COLOR == 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}
// $>>>>>>>>> New

int main()
{
    // This part of code is for window creation
    GLFWwindow *window = initializeAndCreateWindow();

    // $>>>>>>>>> New
    // $>>>>>>>>> New

    // 1. Create program (pipeline)
    // 1.1 Create vertex shader
    unsigned int vertexShader = createShader(GL_VERTEX_SHADER, "./shaders/shader.vert");

    // 1.2 Create vertex shader
    unsigned int fragShader = createShader(GL_FRAGMENT_SHADER, "./shaders/shader.frag");

    // 1.3 Create program and attach the vertex and fragment shaders
    unsigned int shaderProgram = createProgram(vertexShader, fragShader);

    // 2. Define mesh vertices and indices
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.3f, 0.3f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.3f, 0.6f,
        -0.25f, 0.25f, 0.0f, 0.3f, 0.6f, 0.0f,
        0.25f, -0.25f, 0.0f, 0.6f, 0.0f, 0.3f};

    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3};

    // 3. Create vertex array object
    unsigned int VAO = createVAO();

    // 4. Create vertex buffer object and copy vertices to it
    unsigned int VBO = createVBO(vertices, sizeof(vertices));

    // 5. Create element buffer object and copy indices to it
    unsigned int EBO = createEBO(indices, sizeof(indices));

    // 6. Set indices (pointers) to the data in the VBO (aka attributes)
    setAttributesPointers();

    // $>>>>>>>>> New
    // $>>>>>>>>> New

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // $>>>>>>>>> New
        // $>>>>>>>>> New

        // 7. Draw the mesh
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // $>>>>>>>>> New
        // $>>>>>>>>> New

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. Clean up
    glfwTerminate();

    return 0;
}
