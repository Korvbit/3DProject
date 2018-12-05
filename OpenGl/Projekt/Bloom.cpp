#include "Bloom.h"

BloomBuffer::BloomBuffer()
{
	m_fbo = 0;
	m_depthTexture = 0;
}

BloomBuffer::~BloomBuffer()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_textures[0] != 0)
	{
		glDeleteTextures(BLOOMBUFFER_NUM_TEXTURES, m_textures);
	}

	if (m_depthTexture != 0)
	{
		glDeleteTextures(1, &m_depthTexture);
	}
}

void BloomBuffer::Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	unsigned int bloomFBO;
	glGenFramebuffers(1, &bloomFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
	unsigned int colorBuffers[BLOOMBUFFER_NUM_TEXTURES];
	glGenTextures(BLOOMBUFFER_NUM_TEXTURES, colorBuffers);
	for (unsigned int i = 0; i < BLOOMBUFFER_NUM_TEXTURES; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	// Tells openGL that we want two rendertargets
	unsigned int attachments[BLOOMBUFFER_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(BLOOMBUFFER_NUM_TEXTURES, attachments);
}
