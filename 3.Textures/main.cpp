#include "iostream"
#include "fstream"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void sourceShader(GLuint shader, const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
}

GLuint createShader(GLenum type, const std::string &filePath)
{
    GLuint shader;
    shader = glCreateShader(type);
    sourceShader(shader, filePath);
    glCompileShader(shader);
    return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragShader)
{
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glUseProgram(shaderProgram);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return shaderProgram;
}

GLuint createVAO()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    return vao;
}

GLuint createVBO(float *vertices, GLuint size)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    return vbo;
}

GLuint createEBO(GLuint *indices, GLuint size)
{
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    return ebo;
}

void setAttributesPointers()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

// $>>>>>>>>> New - start
// This function is used to set pointers (indices) to the data (attributes) in the VBO
void setAttributesPointers_texture()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

// This function is used to create the texture
GLuint createTexture()
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    return texture;
}

// This function sets the texture parameters
void setWrappingAndFiltering()
{
    // 1. wrapping mode = what to do when the vertex is out of the texture image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 2. filtering mode = which filter to apply to the texture gaps
    //      GL_NEAREST - no interpolation
    //      GL_LINEAR - linear interpolation

    // 2.1. the filter used for minification (zoom out)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Or use mipmap mode for minification (only not for magnification)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // 2.2. the filter used for magnification (zoom in)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// This function is used to load the texture image from the file to the current bound texture
void loadTextureImage(bool alpha, const std::string &imagePath)
{
    int width, height, nrChannels;

    // Flip the image before loading
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        if (alpha)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);
}
// $>>>>>>>>> New - end

int main()
{
    GLFWwindow *window = initializeAndCreateWindow();

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, "./shaders/shader.vert");
    GLuint fragShader = createShader(GL_FRAGMENT_SHADER, "./shaders/shader.frag");
    GLuint shaderProgram = createProgram(vertexShader, fragShader);

    // $>>>>>>>>> New - start
    // 1. Update vbo vertices array to contain the coordinates of each vertex on the texture image [from (0,0) to (1,1)]
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.3f, 0.6f, 1.0f, 1.0f,
        -0.25f, 0.25f, 0.0f, 0.3f, 0.6f, 0.0f, 0.25f, 0.75f,
        0.25f, -0.25f, 0.0f, 0.6f, 0.0f, 0.3f, 0.75f, 0.25f};
    // $>>>>>>>>> New - end

    GLuint indices[] = {
        0, 1, 2,
        0, 1, 3};

    GLuint VAO = createVAO();
    GLuint VBO = createVBO(vertices, sizeof(vertices));
    GLuint EBO = createEBO(indices, sizeof(indices));

    // $>>>>>>>>> New - start
    // 2. Add texture attributes pointers
    // ******
    // ****** UPDATE STRIDE OF ALL OTHER VERTEX ATTRIBUTES ******
    // ******
    setAttributesPointers_texture();

    // 3. Modify the shaders

    // 4. Activate a texture unit
    // A texture unit is a number that links a texture to a sampler
    // So on binding texture object while texture unit x is active, that texture will be linked to the sampler x
    glActiveTexture(GL_TEXTURE0);

    // 5. Create a texture object (to hold the image and parameters of the texture)
    GLuint texture = createTexture();

    // 6. Select texture wrapping and filtering modes
    setWrappingAndFiltering();

    // 7. Load image to the texture object and generate mipmaps
    loadTextureImage(false, "./textures/container.jpg");

    // (Optional) Create another texture
    glActiveTexture(GL_TEXTURE1);
    GLuint texture2 = createTexture();
    loadTextureImage(true, "./textures/awesomeface.png");

    // 8. Define which uniform variable in the fragment shader will be linked to which texture unit (to which sampler)
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);
    // $>>>>>>>>> New - end

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
