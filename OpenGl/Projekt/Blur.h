#pragma once

#include <glew\glew.h>
#include <iostream>

class BlurBuffer
{
public:
	enum BLURBUFFER_TEXTURE_TYPE
	{
		BLURBUFFER_TEXTURE_TYPE_BLUR,
		BLURBUFFER_NUM_TEXTURES
	};

	BlurBuffer();
	~BlurBuffer();

	bool Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);
	void bindForWriting();
	void bindForReading();

private:
	GLuint m_fbo;
	GLuint m_colorBuffers[BLURBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};

