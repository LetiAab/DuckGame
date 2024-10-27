#include "common/foo.h"

#include <iostream>
#include <exception>

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

using namespace SDL2pp;

int main() try {
	/* 
	The client should:
	1. Initilize resources
	2. Update logic
	3. Clean screen
	4. Change frames state
	5. Render screen
	6. Sleep and calc next iteration
	7. Go back to second step

	Remember that we are going to communicate only the changing elements, so there will be a structure that 
	saves last state, to be modified in an intermediate step (Or just the second step).
	*/

	// Initialize SDL library
	SDL sdl(SDL_INIT_VIDEO);

	// Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
	Window window("SDL2pp demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_RESIZABLE);

	// Create accelerated video renderer with default driver
	Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Clear screen
	renderer.Clear();

	// Show rendered frame
	renderer.Present();

	// 5 second delay
	SDL_Delay(5000);

	// Here all resources are automatically released and library deinitialized
	return 0;
} catch (std::exception& e) {
	// If case of error, print it and exit with error
	std::cerr << e.what() << std::endl;
	return 1;
}
