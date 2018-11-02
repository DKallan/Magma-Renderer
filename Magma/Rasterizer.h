#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <algorithm>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "Color.h"
#include "EdgeEquation.h"
#include "ParameterEquation.h"

enum RenderMode
{
	Lines = 1,
	Filled = 2,
	Both = 3
};

enum ViewMode
{
	Orthographic = 1,
	Perspective = 2
};

enum LineColor
{
	Normal = 1,
	Inverted = 2
};

class Rasterizer
{
protected:
	int m_minX, m_minY;
	int m_maxX, m_maxY;
	float* m_zBuffer;
	uint32_t* m_FrameBuffer;
	unsigned int m_Width, m_Height;
	float m_Rotation = 0.0f;
	RenderMode m_RenderMode = RenderMode::Lines;
	ViewMode m_ViewMode = ViewMode::Perspective;
	LineColor m_LineColor = LineColor::Inverted;

public:
	void SetFrameBuffer(uint32_t* frameBuffer, unsigned int width, unsigned int height);
	void SetPixel(unsigned int x, unsigned int y, const Color &color = Color());
	void SetPixel(float x, float y, const Color & color = Color());
	void Clear();

	void DrawLine(const Color &color1, glm::vec3 pointA, const Color &color2, glm::vec3 pointB);
	void DrawTriangle(glm::vec4 vertexA, glm::vec4 vertexB, glm::vec4 vertexC, Color colorA, Color colorB, Color colorC);
	void FillTriangle(glm::vec4 vertexA, glm::vec4 vertexB, glm::vec4 vertexC, Color colorA, Color colorB, Color colorC);
	void DrawVertices(const float*, const int*, int amountOfIndices);

	void IncreaseRotation(float amount);
	void SetRenderMode(RenderMode mode);
	void SetViewMode(ViewMode mode);
	void SetLineColor(LineColor color);

private:
	void ClearZBuffer();
};

#endif