#include "Blur.h"

BlurBuffer::BlurBuffer()
{
	m_fbo = 0;
	m_depthTexture = 0;
}

BlurBuffer::~BlurBuffer()
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_colorBuffers[0] != 0)
	{
		glDeleteTextures(BLURBUFFER_NUM_TEXTURES, m_colorBuffers);
	}

	if (m_depthTexture != 0)
	{
		glDeleteTextures(1, &m_depthTexture);
	}
}

bool BlurBuffer::Init(unsigned int SCREENWIDTH, unsigned int SCREENHEIGHT)
{
	bool finish = true;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glGenTextures(BLURBUFFER_NUM_TEXTURES, m_colorBuffers);
	for (unsigned int i = 0; i < BLURBUFFER_NUM_TEXTURES; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i], 0);
	}


	// Tells openGL that we want two rendertargets
	unsigned int attachments[BLURBUFFER_NUM_TEXTURES] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(BLURBUFFER_NUM_TEXTURES, attachments);

	// Depth texture
	glGenRenderbuffers(1, &m_depthTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREENWIDTH, SCREENHEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTexture);

	// Felcheckar
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		finish = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return finish;
}

void BlurBuffer::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void BlurBuffer::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);

	for (unsigned int i = 0; i < BLURBUFFER_NUM_TEXTURES; i++)
	{
		// if we have different textures to bind, we need to change the current texture openGL is working with.
		glActiveTexture(GL_TEXTURE0 + i);
		// Now when we bind, the bind will affect the current texture that got called by :glActivateTexture
		glBindTexture(GL_TEXTURE_2D, m_colorBuffers[BLURBUFFER_NUM_TEXTURES + i]);
	}
}

