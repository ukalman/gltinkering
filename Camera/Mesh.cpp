#include "Mesh.h"
#include <stdio.h>

Mesh::Mesh() 
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

    glGenVertexArrays(1, &VAO); // (amount of array, the one that you will be working on)
    glBindVertexArray(VAO); // Bind the VAO with that id

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // bind the ibo
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // Instead of sizeof(vertices), we could have also said sizeof(GLfloat) * 9, static draw means we are not going to change the values once we put them in there

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // first one is layout, 0 || second parameter is the size of each value that's going to get passed in (x, y, z in this case) || type of the values || if you want to normalize or no || stride value (if not using multiple VBOS, for ex storing also colors along with positions || offset, where to start from that array, where the data starts
        glEnableVertexAttribArray(0); // layout again. 0

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind it


    glBindVertexArray(0); // unbind it 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // YOU SHOULD UNBIND IBO/EBO AFTER YOU UNBIND THE VAO

}

void Mesh::RenderMesh()
{
    if (VAO == 0 || VBO == 0 || indexCount == 0) return;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh()
{
    if (IBO != 0) 
    {
        glDeleteBuffers(1, &IBO); // to prevent memory overflow. If you delete the Mesh object, the buffer will still be holding, the ID will still connect to that buffer. So if we have no memory management the buffers will stack up, causing in overflow.
        IBO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO); // to prevent memory overflow. If you delete the Mesh object, the buffer will still be holding, the ID will still connect to that buffer. So if we have no memory management the buffers will stack up, causing in overflow.
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO); // to prevent memory overflow. If you delete the Mesh object, the buffer will still be holding, the ID will still connect to that buffer. So if we have no memory management the buffers will stack up, causing in overflow.
        VAO = 0;
    }

    indexCount = 0;
}


Mesh::~Mesh() 
{
    ClearMesh();

}
