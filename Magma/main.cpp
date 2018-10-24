#include <iostream>

#include "SDL.h"

#include "Rasterizer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static bool g_Running = true;

static void HandleKeyEvent(const SDL_Event &event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		g_Running = false;
		break;

	default:
		break;
	}
}

static void HandleEvent(const SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_QUIT:
		g_Running = false;
		break;

	case SDL_KEYDOWN:
		HandleKeyEvent(event);
		break;

	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Init(SDL_INIT_VIDEO);

	// Create an SDL window.
	window = SDL_CreateWindow("Magma Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	if (!window)
	{
		std::cout << "Unable to create a SDL window." << std::endl;
		return -1;
	}

	// Get the window's surface.
	surface = SDL_GetWindowSurface(window);
	if (!surface)
	{
		std::cout << "Unable to get the window's surface." << std::endl;
		return false;
	}

	// Clear the surface to black.
	SDL_FillRect(surface, 0, 0);

	// Copy the surface to the window.
	SDL_UpdateWindowSurface(window);

	// Create a rasterizer and frame buffer.
	Rasterizer rasterizer;
	rasterizer.SetFrameBuffer((uint32_t *)surface->pixels, WINDOW_WIDTH, WINDOW_HEIGHT);

	float r = 0.0f;
	unsigned int lastTicks = SDL_GetTicks();

	while (g_Running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			HandleEvent(event);

		// Lock the surface.
		SDL_LockSurface(surface);

		rasterizer.Clear();
		rasterizer.SetRenderMode(RenderMode::Filled);
		rasterizer.SetViewMode(ViewMode::Perspective);
		rasterizer.SetLineColor(LineColor::Normal);

		// Enable rotation
		bool rotateModel = true;

		// Vertex Buffer
		float vertices[] = {
			// location				// color
			0.5f,  0.5f,  0.5f,		0.0f, 1.0f, 0.0f, // 0 frontTopRight
			0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f, // 1 frontBottomRight
		   -0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 0.0f, // 2 frontBottomLeft
		   -0.5f,  0.5f,  0.5f,		1.0f, 0.5f, 0.0f, // 3 frontTopLeft
		   -0.5f,  0.5f, -0.5f,		1.0f, 0.5f, 0.0f, // 4 backTopLeft
		   -0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f, // 5 backBottomLeft
			0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f, // 6 backBottomRight
			0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 0.0f  // 7 backTopRight
		};

		// Index Buffer
		int indices[] = {
			0, 1, 2,
			0, 2, 3, // front
			3, 2, 5,
			3, 5, 4, // left
			4, 5, 6,
			4, 6, 7, // back
			7, 6, 1, 
			7, 1, 0, // right
			7, 0, 3, 
			7, 3, 4, // top
			1, 6, 5,
			1, 5, 2  // bottom
		};

		// Draw the vertices from the vertex buffer, using the index buffer.
		rasterizer.DrawVertices(vertices, indices, (sizeof(indices) / sizeof(indices[0]) / 3));

		// calculate the number of seconds that
		// have passed since the last update
		unsigned int ticks = SDL_GetTicks();
		unsigned int ticksDiff = ticks - lastTicks;
		if (ticksDiff == 0)
			continue;
		float time = ticksDiff / 1000.0f;
		lastTicks = ticks;

		// update rotation
		if (rotateModel)
			rasterizer.IncreaseRotation((float)M_PI / 2.0f * time);

		// display frames per second
		unsigned int fps = 1000 / ticksDiff;
		printf("Frames per second: %u\t\r", fps);

		// Unlock and update the surface.
		SDL_UnlockSurface(surface);
		SDL_UpdateWindowSurface(window);
	}

	// Let SDL clean up its stuff.
	SDL_Quit();

	return 0;
}