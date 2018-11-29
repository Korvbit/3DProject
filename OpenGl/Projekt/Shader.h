#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>
#include <glew\glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
	Shader();

	void Bind();
	void unBind();
	void Update(const Transform& transform, const Camera& camera);
	void initiateShaders(bool color);

	GLuint CreateShader(const std::string& fileName, GLenum shaderType);
	GLuint *getProgram();

	void sendGBufferVariablesToGPU(const char *name, int value);

	virtual ~Shader();
private:
	unsigned int NUM_OF_SHADERS;

	enum
	{
		TRANSFORM_U,
		WORLD_U,

		NUM_OF_UNIFORMS
	};

	GLuint program;
	GLuint shaders[32];
	GLuint uniforms[32];
};

#endif //SHADER_H