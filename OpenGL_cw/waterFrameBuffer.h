#ifndef WATERFRAMEBUFFERS_H
#define WATERFRAMEBUFFERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class WaterFrameBuffer
{
public:
	WaterFrameBuffer();
	~WaterFrameBuffer();

	// bind custom framebuffer to render reflection
	void bindReflectionFrameBuffer();
	// bind default framebuffer
	void unbindCurrentFrameBuffer();
	// get framebuffer's texture
	unsigned int getReflectionTexture();

private:
	unsigned int reflectionFrameBuffer, reflectionTexture, reflectionRenderBuf;

	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;

	// create and setup framebuffer
	void initReflectionFrameBuffer();
	// function to bind framebuffer
	void bindFrameBuffer(unsigned int frameBuffer, int width, int height);
};

#endif // !WATERFRAMEBUFFERS_H
