#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include <string>
#include "Object.h"

using namespace std;

class ObjectHandler
{
public:
	ObjectHandler();

	Object CreateObject(Mesh *mesh, Transform transform, Texture *texture);

private:
	unsigned int numberOfObjects;
};

#endif