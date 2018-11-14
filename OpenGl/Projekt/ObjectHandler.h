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

	Object CreateObject(Mesh *mesh, Transform transform, Texture *texture);
	Object CreateObject(const char* filePath, Mesh *mesh, Transform transform, Texture *texture);
	bool loadObject(const char * objectPath, vector <glm::vec3> & vertices, vector <glm::vec2> & uvs, vector <glm::vec3> & normals);

private:
	unsigned int numberOfObjects;

	
};

#endif