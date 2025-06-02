#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"

                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\n";

int main()
{
    // --- GLFW WINDOW SETUP --------------------------------------------------

    // initialize glfw
    glfwInit();

    // the first argument tells us what we want to configure
    // the second argument is an int
    // we set the GLFW version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // .3

    // we basically only want the essential features of GLFW
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // something extra for MacOS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // we initialize a window object with the folowing arguments
    // PARAMETERS 1, 2, 3: width | height | title
    // PARAMETER 4: monitor – The monitor to use for full screen mode, or NULL for windowed mode.
    // PARAMETER 5: share – The window whose context to share resources with, or NULL to not share resources.
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // makes the context of the specified window current for the calling thread
    glfwMakeContextCurrent(window);

    // we tell GLFW we want to call the framebuffer_size_callback function on every window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------

    // we use glad to load the function pointers for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // --- VERTEX SHADER ------------------------------------------------------

    // create a vertex shader object and remember it by id
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach the shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compile it
    glCompileShader(vertexShader);

    // check it the compilation worked
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    else
        std::cout << "VERTEX SHADER COMPILED CORRECTLY\n"
                  << std::endl;
    std::cout.flush();

    // --- FRAGMENT SHADER ----------------------------------------------------

    // initialize a fragment shader and remember it by its id
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // attach the shader source code to the shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // compile it
    glCompileShader(fragmentShader);

    // check it the compilation worked
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    else
        std::cout << "FRAGMENT SHADER COMPILED CORRECTLY\n"
                  << std::endl;
    std::cout.flush();

    // --- SHADER PROGRAM -----------------------------------------------------

    // create a shader object and remember it by its id
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // attach the shaders in order and try to link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check it the linking worked
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    else
        std::cout << "SHADER PROGRAM LINKED CORRECTLY\n"
                  << std::endl;
    std::cout.flush();

    // delete the shader objects since we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --- GENERATE VBO, VAO, EBO ---------------------------------------------

    // GET A RECTAGLE

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // get a vertex buffer object and remember it by id
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // initialize a VAO and get it by its id
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // get an EBO and remember it by its id
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // bind the VAO
    glBindVertexArray(VAO);

    // bind the buffer to GL_ARRAY_BUFFER
    // from now on any calls we make on GL_ARRAY_BUFFER will be used to
    // configure the currently bound buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // we get the data from the vertices
    // GL_STATIC_DRAW means the data is: set once, used many times by the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind the buffer and put the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // specify how OpenGL should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // --- RENDER LOOP --------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // render a blue color background
        glClearColor(0.2588f, 0.3529f, 0.9608f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the custom shader program to render the TRIANGLE
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // show in wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(10.0f);
        // draw the poligon
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // revert to fill mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(0);

        // i use this function to process key presses
        processInput(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        // swaps the buffer and displays the pixels to the screen
        glfwSwapBuffers(window);
        // used for triggers like keyboard input or mouse movement
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // ESCAPE (close the window)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // SPACE (make the window red)
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // specify the color to clear the screen with
        // (the state setter)
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        // we use glClear to clear the color buffer of the screen
        // because we only care about the color values we use GL_COLOR_BUFFER_BIT
        // (the state user)
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays

    // 0, 0 means we set the location of the lower left corner of the window

    // we need this because befind the scenes OpenGL uses this data to map coodonates
    // in (-1, 1) x (-1, 1) to (0, width) x (0, height)
    glViewport(0, 0, width, height);
}