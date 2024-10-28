#include "GLWindow.h"

GLWindow::GLWindow()
{
	width = 800;
	height = 600;

    for (size_t i = 0; i < 1024; i++) 
    {
        keys[i] = 0;
    }

    xChange = 0.0f;
    yChange = 0.0f;
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }

    xChange = 0.0f;
    yChange = 0.0f;
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

    // Handle key and mouse input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glfwSetWindowUserPointer(mainWindow, this);
}

void GLWindow::createCallbacks() 
{
    glfwSetKeyCallback(mainWindow, handleKeys); // when a key is pressed on the main window, callback to handleKeys.
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat GLWindow::getXChange() 
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat GLWindow::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}


void GLWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            printf("Pressed: %d\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            printf("Released: %d\n", key);
        }
    }
}

void GLWindow::handleMouse(GLFWwindow* window, double xPos, double yPos) 
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
    
    if (theWindow->mouseFirstMoved)
    {
        // Initialize the starting positions
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos; // prevent the inverted up and down movement

    theWindow->lastX = xPos; 
    theWindow->lastY = yPos;

    printf("x: %.6f, y: %.6f\n", theWindow->xChange, theWindow->yChange);
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}