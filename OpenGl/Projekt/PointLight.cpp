#include "PointLight.h"

PointLightHandler::PointLightHandler()
{
	this->nrOfLights = 0;
}

void PointLightHandler::setLight(glm::vec3 position, glm::vec3 color)
{
	this->pointLight[this->nrOfLights].position = position;
	this->pointLight[this->nrOfLights].color = color;
	this->nrOfLights++;
}

void PointLightHandler::sendToShader()
{
	// Send the lights
	for (int i = 0; i < this->nrOfLights; i++)
	{
		glUniform3f(loc_position[i], this->pointLight[i].position.x, this->pointLight[i].position.y, this->pointLight[i].position.z);
		glUniform3f(loc_color[i], this->pointLight[i].color.x, this->pointLight[i].color.y, this->pointLight[i].color.z);
	}

	// Send the nrOfLights variable
	glUniform1i(this->loc_NrOfLights, this->nrOfLights);
}

void PointLightHandler::initiateLights(GLuint *program)
{
	char name[128];
	
	// Tell the gpu the names of the uniforms
	for (int i = 0; i < this->nrOfLights; i++)
	{
		memset(name, 0, sizeof(name));
		snprintf(name, sizeof(name), "PointLights[%d].position", i);
		loc_position[i] = glGetUniformLocation(*program, name);

		memset(name, 0, sizeof(name));
		snprintf(name, sizeof(name), "PointLights[%d].color", i);
		loc_color[i] = glGetUniformLocation(*program, name);
	}

	this->loc_NrOfLights = glGetUniformLocation(*program, "NR_OF_POINT_LIGHTS");
}

PointLightHandler::~PointLightHandler()
{
}