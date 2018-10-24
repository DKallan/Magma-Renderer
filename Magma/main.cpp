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

	while (g_Running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			HandleEvent(event);

		// Lock the surface.
		SDL_LockSurface(surface);

		rasterizer.Clear();
		rasterizer.SetRenderMode(RenderMode::Both);
		
		glm::vec4 pointA = glm::vec4(0.75f, 0.75f, 1.0f, 1.0f);
		glm::vec4 pointB = glm::vec4(0.75f, 0.25f, 1.0f, 1.0f);
		glm::vec4 pointC = glm::vec4(0.25f, 0.25f, 1.0f, 1.0f);
		glm::vec4 pointD = glm::vec4(0.25f, 0.75f, 1.0f, 1.0f);

		rasterizer.DrawTriangle(pointA, pointB, pointC, Color::red(), Color::green(), Color::blue());
		rasterizer.DrawTriangle(pointA, pointC, pointD, Color::red(), Color::blue(), Color::orange());

		// Unlock and update the surface.
		SDL_UnlockSurface(surface);
		SDL_UpdateWindowSurface(window);
	}

	// Let SDL clean up its stuff.
	SDL_Quit();

	return 0;
}