#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
		cameraPosition = pos;
		forwardVector = glm::vec3(0, 0, 1);
		upVector = glm::vec3(0, 1, 0);
		viewMatrix = glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
	}

	inline glm::mat4 getViewProjection() const
	{
		return projectionMatrix * viewMatrix;
	}

	/*
	inline glm::vec3 setCameraPosition(glm::vec3 pos)
	{
		cameraPosition = pos;
		viewMatrix = glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
	}*/


	virtual ~Camera()
	{

	}

private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 forwardVector;
	glm::vec3 upVector;

};

#endif