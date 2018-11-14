#include "ObjectHandler.h"

ObjectHandler::ObjectHandler()
{
	this->numberOfObjects = 0;
}

Object ObjectHandler::CreateObject(Mesh *mesh, Transform transform, Texture *texture)
{
	return Object(mesh, transform, texture, this->numberOfObjects++);
}

Object ObjectHandler::CreateObject(const char* filePath, Mesh *mesh, Transform transform, Texture *texture)
{
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvCoords;
	vector<glm::vec3> normals;

	bool loaded = false;
	loaded = loadObject(filePath, vertices, uvCoords, normals);

	if (loaded == true)
	{
		std::cout << "The object has been loaded correctly!" << std::endl;
	}
	else
	{
		std::cout << "It went to fralleballes balls!" << std::endl;
	}

	mesh->createMesh(vertices, uvCoords);

	return Object(mesh, transform, texture, this->numberOfObjects++);
}

bool ObjectHandler::loadObject(const char * objectPath, vector<glm::vec3>& vertices, vector<glm::vec2>& uvs, vector<glm::vec3>& normals)
{
	bool objectLoaded = false;
	bool endOfFile = false;

	// Vectors to hold the indexes of the faces in the objFile.
	vector<unsigned int> vertexIndices;
	vector<unsigned int> uvIndices;
	vector<unsigned int> normalIndices;

	// Vectors to temporary store the attributes of each face.
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	// Read the filePath and check if we can open it
	FILE * file = fopen(objectPath, "r");
	if (file == NULL)
	{
		std::cout << "[ERROR] - Cannot open the objectFile" << std::endl;
		objectLoaded = false;
	}
	else
	{
		// Read the file and save the data
		while (endOfFile == false)
		{
			char firstWordOfLine[128];
			int wordSize = fscanf(file, "%s", firstWordOfLine);
			if (wordSize == EOF)
			{
				// There is nothing left to be read
				objectLoaded = true;
				endOfFile = true;
			}
			else
			{
				// Find the value of each "v", "vt", "vt" or "f" and store them in the temp vectors
				if (strcmp(firstWordOfLine, "v") == 0)
				{
					glm::vec3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(firstWordOfLine, "vt") == 0)
				{
					glm::vec2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					temp_uvs.push_back(uv);
				}
				else if (strcmp(firstWordOfLine, "vn") == 0)
				{
					glm::vec3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(firstWordOfLine, "f") == 0)
				{
					// find all the indices
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int nrOfIndices = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
										&vertexIndex[0], &uvIndex[0], &normalIndex[0],
										&vertexIndex[1], &uvIndex[1], &normalIndex[1],
										&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					// Check if we're dealing with a hard af object file
					if (nrOfIndices != 9)
					{
						// Our object loader is to simple to be able to deal with an obj file like this
						std::cout << "[ERROR] To complicated obj file!!" << std::endl;
						objectLoaded = false;
						break;
					}

					// Store the indices
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
			}
		}
		
		unsigned int vertexIndex = 0;
		unsigned int uvIndex = 0;
		unsigned int normalIndex = 0;
		glm::vec3 vertex;
		glm::vec2 uv;
		glm::vec3 normal;
		// Now we store the attributes in the i
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			// Find the index
			vertexIndex = vertexIndices[i];
			uvIndex = uvIndices[i];
			normalIndex = normalIndices[i];

			// Find the value of that index	(-1 because all object files starts their indexing at 1 instead of 0)
			vertex = temp_vertices[vertexIndex - 1];
			uv = temp_uvs[uvIndex - 1];
			normal = temp_normals[normalIndex - 1];

			// Store the values in the vectors we recieve in the function
			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
		}
	}
	return objectLoaded;
}
