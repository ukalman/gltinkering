#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices); // first two parameters are unsigned int arrays
	void RenderMesh();
	void ClearMesh();


	~Mesh();


private:
	GLuint VAO, VBO, IBO; // you actually don't need VBO, and IBO is needed for some probable bugs that might occur in some graphics cards
	GLsizei indexCount;
};

