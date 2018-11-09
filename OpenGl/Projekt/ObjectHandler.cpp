#include "ObjectHandler.h"

ObjectHandler::ObjectHandler()
{
	this->numberOfObjects = 0;
}

Object ObjectHandler::CreateObject(Mesh *mesh, Transform transform, Texture *texture)
{
	return Object(mesh, transform, texture, this->numberOfObjects++);
}
