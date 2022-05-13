#include "waterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer()
{
	initReflectionFrameBuffer();
}

WaterFrameBuffer::~WaterFrameBuffer()
{
	glad_glDeleteFramebuffers(1, &reflectionFrameBuffer);
}

void WaterFrameBuffer::bindReflectionFrameBuffer()
{
	bindFrameBuffer(reflectionFrameBuffer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void WaterFrameBuffer::initReflectionFrameBuffer()
{
	glGenFramebuffers(1, &reflectionFrameBuffer);

	// bind reflection texture
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);

	// create render buffer for depth and stencil
	glGenRenderbuffers(1, &reflectionRenderBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionRenderBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
		reflectionRenderBuf);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterFrameBuffer::unbindCurrentFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int WaterFrameBuffer::getReflectionTexture()
{
	return reflectionTexture;
}

void WaterFrameBuffer::bindFrameBuffer(unsigned int frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

