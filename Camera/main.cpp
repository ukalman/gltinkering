#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

/*
bool direction = true; // right = false
float triOffset = 0.0f;
float triMax = 0.7f;
float triIncrement = 0.005f;

float currentAngle = 0.0f;

bool sizeDirection = true; // true is scaling up
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
*/

const float toRadians = 3.14159265f / 180.0f; // rotation functions uses radians

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


// Vertex Shader (defined as string - const char array)
static const char* vShader = "Shaders/shader.vert";

static const char* fShader = "Shaders/shader.frag";

void CreateTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, // is black when converted to rgb, after clamping it to (0,0,0)
        1.0f, -1.0f, 0.0f, // is red when converted to rgb, after clamping it to (1,0,0)
        0.0f, 1.0f, 0.0f // is green when converted to rgb, (0,1,0)
    }; 
}

void CreateTrianglePyramids()
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

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
    
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}



int main()
{

    mainWindow = GLWindow(800, 600);
    mainWindow.Initialize();
    
    //CreateTriangle();
    CreateTrianglePyramids();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);


    // Loop until window closed
    while (!mainWindow.getShouldClose()) 
    {
        GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
        deltaTime = now - lastTime; // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency();
        lastTime = now;

        // Get + Handle user input events
        glfwPollEvents(); // checks if any event occurred (keyboard press, mouse movement, clicking the cross top right, moving the window around the screen, resizing the window, etc.)

        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange()); 

        /*
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
        */

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer
         
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();

            glm::mat4 model(1.0f);
        
            //model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
            //model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around z axis, if it comes before translation, you can imagine the world is being rotated so axes directions changed. 
            //model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
            //model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y axis

            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

            //glUniform1f(uniformXMove, triOffset);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); // third parameter is transpose
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

            meshList[0]->RenderMesh();

            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(triOffset, 1.0f, -2.5f));
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
            meshList[1]->RenderMesh();


        glUseProgram(0); // Unassign the shader

        mainWindow.SwapBuffers(); // Swaps the front and back buffers to display the new content on the screen.
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
