#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include <string>
#include "Object.h"
#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

class ObjectHandler
{
public:
	ObjectHandler();

	int CreateObject(Mesh *mesh, Transform transform, Texture *texture);
	int CreateObject(const char* filePath, Mesh *mesh, Transform transform, Texture *texture);
	bool loadObject(const char * objectPath, vector <glm::vec3> & vertices, vector <glm::vec2> & uvs, vector <glm::vec3> & normals);

	unsigned int getNrOfObjects();
	Object* getObject(int index);

private:
	unsigned int numberOfObjects;
	Object allObjects[256];

	
};

#endif