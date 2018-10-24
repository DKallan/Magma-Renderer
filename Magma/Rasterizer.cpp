#include "Rasterizer.h"

/// Set the frame buffer with the given dimensions.
void Rasterizer::SetFrameBuffer(uint32_t *frameBuffer, unsigned int width, unsigned int height)
{
	m_FrameBuffer = frameBuffer;
	m_Width = width;
	m_Height = height;
	m_maxX = width;
	m_maxY = height;
}

/// Set the color value for the given pixel.
void Rasterizer::SetPixel(unsigned int x, unsigned int y, const Color &color)
{
	if (x >= m_Width || y >= m_Height)
		return;

	m_FrameBuffer[y * m_Width + x] = color.ToUInt32();
}

/// Set the color value for the given pixel.
void Rasterizer::SetPixel(float x, float y, const Color &color)
{
	if (x < 0.0f || y < 0.0f)
		return;

	SetPixel((unsigned int)x, (unsigned int)y, color);
}

/// Clear the framebuffer.
void Rasterizer::Clear()
{
	memset(m_FrameBuffer, 0, sizeof(uint32_t) * m_Height * m_Width);
}

/// Draw a line between the first point and the second point.
void Rasterizer::DrawLine(const Color &color1, float x1, float y1, const Color &color2, float x2, float y2)
{
	float xdiff = (x2 - x1);
	float ydiff = (y2 - y1);

	// if the points are the same, simply draw one point.
	if (xdiff == 0.0f && ydiff == 0.0f)
	{
		SetPixel(x1, y1, color1);
		return;
	}

	if (fabs(xdiff) > fabs(ydiff))
	{
		float xmin, xmax;

		// check which point comes first on the x-axis.
		if (x1 < x2)
		{
			xmin = x1;
			xmax = x2;
		}
		else
		{
			xmin = x2;
			xmax = x1;
		}

		float slope = ydiff / xdiff;
		for (float x = xmin; x <= xmax; x += 1.0f)
		{
			float y = y1 + ((x - x1) * slope);
			Color color = color1 + ((color2 - color1) * ((x - x1) / xdiff));
			SetPixel(x, y, color);
		}
	}
	else
	{
		float ymin, ymax;

		// check which point comes first on the y-axis.
		if (y1 < y2)
		{
			ymin = y1;
			ymax = y2;
		}
		else
		{
			ymin = y2;
			ymax = y1;
		}

		float slope = xdiff / ydiff;
		for (float y = ymin; y <= ymax; y += 1.0f)
		{
			float x = x1 + ((y - y1) * slope);
			Color color = color1 + ((color2 - color1) * ((y - y1) / ydiff));
			SetPixel(x, y, color);
		}
	}
}

void Rasterizer::DrawTriangle(glm::vec4 vertexA, glm::vec4 vertexB, glm::vec4 vertexC, Color colorA, Color colorB, Color colorC)
{
	// Reverse the y coordinate so we draw from the bottom left to the top right.
	vertexA.y = -vertexA.y;
	vertexB.y = -vertexB.y;
	vertexC.y = -vertexC.y;
	
	glm::mat4 projectionMatrix, viewMatrix, model;

	if(m_ViewMode == ViewMode::Orthographic)
		projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 2.0f, 100.0f);
	else
		projectionMatrix = glm::perspective(glm::radians(90.0f), ((float)m_Width / (float)m_Height), 0.1f, 100.0f);

	viewMatrix = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));

	// Apply all matrixes.
	vertexA = projectionMatrix * viewMatrix * model * vertexA;
	vertexB = projectionMatrix * viewMatrix * model * vertexB;
	vertexC = projectionMatrix * viewMatrix * model * vertexC;

	// ndc space (normalized device coordinates)
	vertexA = (vertexA + glm::vec4(1.0f, 1.0f, 0.0f, 0.0f)) / 2.0f;
	vertexB = (vertexB + glm::vec4(1.0f, 1.0f, 0.0f, 0.0f)) / 2.0f;
	vertexC = (vertexC + glm::vec4(1.0f, 1.0f, 0.0f, 0.0f)) / 2.0f;

	// Screen space pixels.
	vertexA.x = vertexA.x * m_Width;
	vertexA.y = vertexA.y * m_Height;
	vertexB.x = vertexB.x * m_Width;
	vertexB.y = vertexB.y * m_Height;
	vertexC.x = vertexC.x * m_Width;
	vertexC.y = vertexC.y * m_Height;

	// Fill the triangle if specified to do so.
	if (m_RenderMode == RenderMode::Filled || m_RenderMode == RenderMode::Both)
		FillTriangle(vertexA, vertexB, vertexC, colorA, colorB, colorC);

	if (m_RenderMode == RenderMode::Lines || m_RenderMode == RenderMode::Both)
	{
		// Draw the wireframe in white
		DrawLine(colorA, vertexA.x, vertexA.y, colorB, vertexB.x, vertexB.y);
		DrawLine(colorB, vertexB.x, vertexB.y, colorC, vertexC.x, vertexC.y);
		DrawLine(colorC, vertexC.x, vertexC.y, colorA, vertexA.x, vertexA.y);
	}
}


void Rasterizer::FillTriangle(glm::vec4 vertexA, glm::vec4 vertexB, glm::vec4 vertexC, Color colorA, Color colorB, Color colorC)
{
	// Compute triangle bounding box. 
	int minX = std::min(std::min(vertexA.x, vertexB.x), vertexC.x);
	int maxX = std::max(std::max(vertexA.x, vertexB.x), vertexC.x);
	int minY = std::min(std::min(vertexA.y, vertexB.y), vertexC.y);
	int maxY = std::max(std::max(vertexA.y, vertexB.y), vertexC.y);

	// Clip to scissor rect. 
	minX = std::max(minX, m_minX);
	maxX = std::min(maxX, m_maxX);
	minY = std::max(minY, m_minY);
	maxY = std::min(maxY, m_maxY);
	maxY = std::min(maxY, m_maxY);

	// Compute edge equations. 
	EdgeEquation e0(vertexA, vertexB);
	EdgeEquation e1(vertexB, vertexC);
	EdgeEquation e2(vertexC, vertexA);

	float area = 0.5 * (e0.c + e1.c + e2.c);
	
	// Compute equations for color.
	ParameterEquation R(colorC.R, colorA.R, colorB.R, e0, e1, e2, area);
	ParameterEquation G(colorC.G, colorA.G, colorB.G, e0, e1, e2, area);
	ParameterEquation B(colorC.B, colorA.B, colorB.B, e0, e1, e2, area);

	//Check if triangle is backfacing. 
	if (area < 0)
		return;

	// Add 0.5 to sample at pixel centers. 
	for (float x = minX + 0.5f, xm = maxX + 0.5f; x <= xm; x += 1.0f)
	{
		for (float y = minY + 0.5f, ym = maxY + 0.5f; y <= ym; y += 1.0f)
		{
			if (e0.test(x, y) && e1.test(x, y) && e2.test(x, y))
			{
				float r = R.evaluate(x, y);
				float g = G.evaluate(x, y);
				float b = B.evaluate(x, y);
				SetPixel(x, y, Color(r, g, b));
			}
		}
	}
}

void Rasterizer::DrawVertices(const float* vertices, const int* indices, int amountOfIndices)
{
	for (int i = 0; i < amountOfIndices; i++)
	{
		glm::vec4 pointA = glm::vec4(vertices[indices[i * 3]	 * 6], vertices[indices[i * 3]	   * 6 + 1], vertices[indices[i * 3]	 * 6 + 2], 1.0f);
		glm::vec4 pointB = glm::vec4(vertices[indices[i * 3 + 1] * 6], vertices[indices[i * 3 + 1] * 6 + 1], vertices[indices[i * 3 + 1] * 6 + 2], 1.0f);
		glm::vec4 pointC = glm::vec4(vertices[indices[i * 3 + 2] * 6], vertices[indices[i * 3 + 2] * 6 + 1], vertices[indices[i * 3 + 2] * 6 + 2], 1.0f);

		Color colorA = Color(vertices[indices[i * 3]	 * 6 + 3], vertices[indices[i * 3]	   * 6 + 4], vertices[indices[i * 3]	 * 6 + 5]);
		Color colorB = Color(vertices[indices[i * 3 + 1] * 6 + 3], vertices[indices[i * 3 + 1] * 6 + 4], vertices[indices[i * 3 + 1] * 6 + 5]);
		Color colorC = Color(vertices[indices[i * 3 + 2] * 6 + 3], vertices[indices[i * 3 + 2] * 6 + 4], vertices[indices[i * 3 + 2] * 6 + 5]);

		DrawTriangle(pointA, pointB, pointC, colorA, colorB, colorC);
	}
}

void Rasterizer::IncreaseRotation(float amount)
{
	m_Rotation += amount;
}

void Rasterizer::SetRenderMode(RenderMode mode)
{
	m_RenderMode = mode;
}

void Rasterizer::SetViewMode(ViewMode mode)
{
	m_ViewMode = mode;
}