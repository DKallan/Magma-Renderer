#include "Rasterizer.h"

/// Set the frame buffer with the given dimensions.
void Rasterizer::SetFrameBuffer(uint32_t *frameBuffer, unsigned int width, unsigned int height)
{
	m_FrameBuffer = frameBuffer;
	m_Width = width;
	m_Height = height;
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

void Rasterizer::IncreaseRotation(float amount)
{
	m_Rotation += amount;
}

void Rasterizer::SetRenderMode(RenderMode rendermode)
{
	m_RenderMode = rendermode;
}