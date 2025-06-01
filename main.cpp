#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------

    // initialize GLFW
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

    // glfw window creation
    // --------------------

    // we initialize a window object with the folowing arguments
    // PARAMETERS 1, 2, 3: width | height | title
    // PARAMETER 4: monitor – The monitor to use for full screen mode, or NULL for windowed mode.
    // PARAMETER 5: share – The window whose context to share resources with, or NULL to not share resources.
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    // if this did not work we throw an error
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        // i use this function to exit on Esc key press
        processInput(window);

        // render
        // ------
        // R, G, B, transparency
        glClearColor(0.2588f, 0.3529f, 0.9608f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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