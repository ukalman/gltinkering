#include "GLWindow.h"

GLWindow::GLWindow()
{
	width = 800;
	height = 600;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}

int GLWindow::Initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("GLFW Initialization Failed!");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW Window properties
    // OpenGL version (3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL .3
    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // How OpenGL will treat the code. Core Profile means that it won't be backwards compatible, meaning that we don't want to use deprecated features.
    // Allow Forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Text Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW Window Creation Failed!");
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size Information - buffer, holds all the OpenGL data as it's being passed to the window
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow); // If you have multiple windows, you can switch between which window is the current context for OpenGL draws


    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization Failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST); // Enables depth testing to determine which triangles are deeper into the image effectively, and therefore which ones it should draw over the top of the others

    // Setup Viewport Size
    glViewport(0, 0, bufferWidth, bufferHeight); // make the entire part of our window this viewport.
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}