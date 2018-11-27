#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <iostream>
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <stdio.h>
using namespace std;

const int MAX_NUMBER_OF_LIGHTS = 256;
struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
};

class PointLightHandler
{
private:
	PointLight pointLight[MAX_NUMBER_OF_LIGHTS];
	GLuint nrOfLights;

	GLuint loc_position[MAX_NUMBER_OF_LIGHTS];
	GLuint loc_color[MAX_NUMBER_OF_LIGHTS];

	GLuint loc_NrOfLights;
public:
	PointLightHandler();

	// location, pos, color
	void setLight(glm::vec3 position, glm::vec3 color);

	void sendToShader();
	void initiateLights(GLuint *program);
	virtual ~PointLightHandler();
};

#endif