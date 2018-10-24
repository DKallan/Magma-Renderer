#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "glm.hpp"

#include "Color.h"

enum RenderMode
{
	Lines = 1,
	Filled = 2,
	Both = 3
};

class Rasterizer
{
protected:
	uint32_t *m_FrameBuffer;
	unsigned int m_Width, m_Height;
	float m_Rotation = 0.0f;
	RenderMode m_RenderMode;

public:
	void SetFrameBuffer(uint32_t* frameBuffer, unsigned int width, unsigned int height);
	void SetPixel(unsigned int x, unsigned int y, const Color &color = Color());
	void SetPixel(float x, float y, const Color & color = Color());
	void Clear();

	void DrawLine(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2);

	void IncreaseRotation(float amount);
	void SetRenderMode(RenderMode);
};

#endif