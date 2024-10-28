#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat getBufferWidth() { return bufferWidth;  }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();


	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	~GLWindow();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // covers the range of ASCII characters

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved; // need to check the very first movement (when the application starts, according to the position of the mouse, it could behave as if we moved the mouse so we might get this moment where suddenly the screen flicks to one side or the camera violently turns right at the start


	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); // need to set this static because GLFW will be doing a callback and we need a static function for it to callback to you because a regular member/just a normal member functions are not going to work in this case.
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

};

