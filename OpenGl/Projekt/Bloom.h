#ifndef BLOOM_H
#define BLOOM_H

#include <glew\glew.h>
#include <iostream>

class BloomBuffer
{
public:
	enum BLOOMBUFFER_TEXTURE_TYPE
	{
		BLOOMBUFFER_TEXTURE_TYPE_BLOOMMAP,
		BLOOMBUFFER_TEXTURE_TYPE_DIFFUSE,
		BLOOMBUFFER_NUM_TEXTURES
	};

	BloomBuffer();
	~BloomBuffer();

	void Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT);

private:
	GLuint m_fbo;
	GLuint m_textures[BLOOMBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};

#endif