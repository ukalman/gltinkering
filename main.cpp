#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint VAO, VBO,IBO, shader, uniformProjection, uniformModel; // Usually one would have multiple vaos and vbos for each object

bool direction = true; // right = false
float triOffset = 0.0f;
float triMax = 0.7f;
float triIncrement = 0.005f;

float currentAngle = 0.0f;

bool sizeDirection = true; // true is scaling up
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;


// Window Dimensions
const GLint WIDTH = 800, HEIGHT = 600; // 800:600 window
const float toRadians = 3.14159265f / 180.0f; // rotation functions uses radians

// Vertex Shader (defined as string - const char array)
static const char* vShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
out vec4 vColor;                                                \n\
                                                                \n\
uniform mat4 model;                                             \n\
uniform mat4 projection;                                        \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    gl_Position = projection * model * vec4(pos, 1.0);          \n\
    vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                \n\
}";

static const char* fShader = "                                  \n\
#version 330                                                    \n\
                                                                \n\
in vec4 vColor;                                                 \n\
                                                                \n\
out vec4 color;                                                 \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    color = vColor;                                             \n\
}";

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, // is black when converted to rgb, after clamping it to (0,0,0)
        1.0f, -1.0f, 0.0f, // is red when converted to rgb, after clamping it to (1,0,0)
        0.0f, 1.0f, 0.0f // is green when converted to rgb, (0,1,0)
    };

    // Create a Vertex Array
    glGenVertexArrays(1, &VAO); // (amount of array, the one that you will be working on)
    glBindVertexArray(VAO); // Bind the VAO with that id

        // Some people use indentation here, because now we're working with the VAO above.
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW); // Instead of sizeof(vertices), we could have also said sizeof(GLfloat) * 9, static draw means we are not going to change the values once we put them in there

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // first one is layout, 0 || second parameter is the size of each value that's going to get passed in (x, y, z in this case) || type of the values || if you want to normalize or no || stride value (if not using multiple VBOS, for ex storing also colors along with positions || offset, where to start from that array, where the data starts
            glEnableVertexAttribArray(0); // layout again. 0

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind it

    glBindVertexArray(0); // unbind it 

}

void CreateTrianglePyramid()
{
    // for indexed draw (we don't need to restate the coordinates and define all the vertices one-by-one)
    unsigned int indices[] = {
        0, 3, 1, // one side
        1, 3, 2, // the other side
        2, 3, 0, // front facing side
        0, 1, 2 // base
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, // bottom left coordinate
        0.0f, -1.0f, 1.0f, // this one will go into the background
        1.0f, -1.0f, 0.0f, // bottom right coordinate
        0.0f, 1.0f, 0.0f // top coordinate
    };

    // Create a Vertex Array
    glGenVertexArrays(1, &VAO); // (amount of array, the one that you will be working on)
    glBindVertexArray(VAO); // Bind the VAO with that id

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // bind the ibo
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Instead of sizeof(vertices), we could have also said sizeof(GLfloat) * 9, static draw means we are not going to change the values once we put them in there

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // first one is layout, 0 || second parameter is the size of each value that's going to get passed in (x, y, z in this case) || type of the values || if you want to normalize or no || stride value (if not using multiple VBOS, for ex storing also colors along with positions || offset, where to start from that array, where the data starts
        glEnableVertexAttribArray(0); // layout again. 0

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind it
        

    glBindVertexArray(0); // unbind it 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // YOU SHOULD UNBIND IBO/EBO AFTER YOU UNBIND THE VAO

}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType); // Create an empty shader of type

    const GLchar* theCode[1]; // where we store the code itself
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode); // length of the actual code

    glShaderSource(theShader, 1, theCode, codeLength); // the second parameter is count. You can have multiple strings. We could have lots of chunks of code and then combine them all into this one mass load of source for the code, here though, we've only got one file we're passing in.
    glCompileShader(theShader);

    // Error check
    GLint result = 0; 
    GLchar eLog[1024] = { 0 }; // a place to log the error

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); // get information

    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog); // get the info log for the program, pass it into the log char array eLog
        printf("Error Compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, theShader);

}



void CompileShaders() {
    shader = glCreateProgram(); // generate shader program id

    if (!shader) {
        printf("Error creating shader program\n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    // Error check
    GLint result = 0; // result of the two functions we're about to perform
    GLchar eLog[1024] = { 0 }; // a place to log the error

    glLinkProgram(shader); // Create the executables on the graphics card
    glGetProgramiv(shader, GL_LINK_STATUS, &result); // get information

    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); // get the info log for the program, pass it into the log char array eLog
        printf("Error Linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader); // checks if the shader which we've now created is valid in the current context that OpenGL is working in
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result); // get information

    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog); // get the info log for the program, pass it into the log char array eLog
        printf("Error Validating program: '%s'\n", eLog);
        return;
    }

    //uniformXMove = glGetUniformLocation(shader, "xMove"); //first one is the shader program
    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");

}


int main()
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
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); // How OpenGL will treat the code. Core Profile means that it won't be backwards compatible, meaning that we don't want to use deprecated features.
    // Allow Forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Text Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW Window Creation Failed!");
        glfwTerminate();
        return 1;
    }

    // Get Buffer Size Information - buffer, holds all the OpenGL data as it's being passed to the window
    int bufferWidth, bufferHeight;
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

    //CreateTriangle();
    CreateTrianglePyramid();
    CompileShaders();

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);


    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) 
    {
        // Get + Handle user input events
        glfwPollEvents(); // checks if any event occurred (keyboard press, mouse movement, clicking the cross top right, moving the window around the screen, resizing the window, etc.)

        if (direction)
        {
            triOffset += triIncrement;
        }
        else {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMax)
        {
            direction = !direction;
        }

        currentAngle += 0.1f;
        if (currentAngle >= 360)
        {
            currentAngle -= 360;
        }

        if (sizeDirection)
        {
            currentSize += 0.01f;
        }
        else {
            currentSize -= 0.01f;
        }

        if (currentSize >= maxSize || currentSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }


        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer
         
        glUseProgram(shader);

            glm::mat4 model(1.0f);
        
            //model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
            //model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around z axis, if it comes before translation, you can imagine the world is being rotated so axes directions changed. 
            model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
            //model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y axis
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

            //glUniform1f(uniformXMove, triOffset);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // third parameter is transpose
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
             
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glUseProgram(0); // Unassign the shader

        glfwSwapBuffers(mainWindow); // Swaps the front and back buffers to display the new content on the screen.
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
