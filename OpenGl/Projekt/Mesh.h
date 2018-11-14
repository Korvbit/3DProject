#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew\glew.h>
#include <vector>

class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
	}

	inline glm::vec3* GetPos() { return &pos; }
	inline glm::vec2* GetTexCoord() { return &texCoord; }
private:
	glm::vec3 pos;
	glm::vec2 texCoord;
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numOfVertices);
	Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords);
	Mesh() {}
	void Draw();

	void operator=(const Mesh& other) {}

	virtual ~Mesh();
	void createMesh(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvCoords);
private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,

		NUM_OF_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_OF_BUFFERS];
	unsigned int drawCount;
};

#endif //MESH_H