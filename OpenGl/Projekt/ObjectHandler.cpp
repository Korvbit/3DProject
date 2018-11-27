#include "ObjectHandler.h"

ObjectHandler::ObjectHandler()
{
	this->numberOfObjects = 0;
}

int ObjectHandler::CreateObject(Mesh *mesh, Transform transform, Texture *texture)
{
	this->allObjects[this->numberOfObjects] = Object(mesh, transform, texture, this->numberOfObjects);

	return this->numberOfObjects++;
}

int ObjectHandler::CreateObject(const char* filePath, Mesh *mesh, Transform transform, Texture *texture)
{
	bool loaded = false;
	loaded = mesh->createMesh(filePath);

	if (loaded == true)
	{
		std::cout << "The object has been loaded correctly!" << std::endl;
	}
	else
	{
		std::cout << "Bad things happened during the attempt to load the object!" << std::endl;
	}

	this->allObjects[this->numberOfObjects] = Object(mesh, transform, texture, this->numberOfObjects);

	return this->numberOfObjects++;
}


unsigned int ObjectHandler::getNrOfObjects()
{
	return this->numberOfObjects;
}

Object* ObjectHandler::getObject(int index)
{
	return &this->allObjects[index];
}
