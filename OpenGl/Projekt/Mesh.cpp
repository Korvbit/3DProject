#include "Mesh.h"


Mesh::Mesh(Vertex* vertices, unsigned int numOfVertices)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	positions.reserve(numOfVertices);
	texCoords.reserve(numOfVertices);

	for (unsigned int i = 0; i < numOfVertices; i++)
	{
		positions.push_back(*vertices[i].GetPos());
		texCoords.push_back(*vertices[i].GetTexCoord());

	}

	createMesh(positions, texCoords);
}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords)
{
	createMesh(vertices, uvCoords);
}

void Mesh::Draw()
{
	glBindVertexArray(vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, drawCount);

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::createMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords)
{
	drawCount = vertices.size();

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(NUM_OF_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, uvCoords.size() * sizeof(uvCoords[0]), &uvCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}
