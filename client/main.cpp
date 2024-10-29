#include "common/foo.h"

#include <iostream>
#include <exception>

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "client.h"

using namespace SDL2pp;

#define ERROR 1
#define SUCCESS 0

struct Duck {
	int x, y;
	SDL_RendererFlip flipType;
};

struct Crate {
	int x, y;
};

struct GameState {
	Duck duck;
	Crate crates[4];
	SDL_Texture *crate, *duck_t, *background;
	SDL_Renderer* renderer;
};

SDL_Surface* loadImage(const std::string& name_img) {
	SDL_Surface* img = NULL;
	const std::string path = std::string(IMAGE_PATH) + name_img + ".png";
	img = IMG_Load(path.c_str());
	if (img == NULL) {
		std::cerr << "Error loading " << name_img << ".png: " << IMG_GetError() << "\n";
		SDL_Quit();
		exit(ERROR);
	}
	return img;
}

void loadGame(GameState* game) {
	// Inicializo el background
	SDL_Surface* background = loadImage("forest");
	game->background = SDL_CreateTextureFromSurface(game->renderer, background);
	SDL_FreeSurface(background);

	// Inicializo el pato
	game->duck.x = 280;
	game->duck.y = 300;
	game->duck.flipType = SDL_FLIP_NONE;
	SDL_Surface* duck_surface = loadImage("duck");
	game->duck_t = SDL_CreateTextureFromSurface(game->renderer, duck_surface);
	SDL_FreeSurface(duck_surface);

	// Inicializo crate
	SDL_Surface* crate_surface = loadImage("crate");
	game->crate = SDL_CreateTextureFromSurface(game->renderer, crate_surface);
	SDL_FreeSurface(crate_surface);

	for (int i=0; i<4; i++) {
		game->crates[i].x = 400 + i * 50;
		game->crates[i].y = 300;
	}

}

int processEvents(SDL_Window* window, GameState* game) {
	int done = 0;
	SDL_Event event;
	//SDL_PollEvent(&event) devuelve true si hay algun evento que procesar
	while(SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_WINDOWEVENT_CLOSE:
				if (window) {
					SDL_DestroyWindow(window);
					window = NULL;
					done = 1;
				}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						done = 1;
					break;
					default: break;
				}
			break;
			case SDL_QUIT:
				done = 1;
			break;
			default: break;
		}
	}

	const uint8_t* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LEFT]) {
		game->duck.x--;
		game->duck.flipType = SDL_FLIP_HORIZONTAL;
	}
	if (state[SDL_SCANCODE_RIGHT]) {
		game->duck.x++;
		game->duck.flipType = SDL_FLIP_NONE;
	}
	if (state[SDL_SCANCODE_UP]) {
		game->duck.y--;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		game->duck.y++;
	}

	return done;
}

void doRender(SDL_Renderer* renderer, GameState* game) {
	//Render display
	SDL_RenderCopy(renderer, game->background, NULL, NULL);

	// Dibujo pato
	SDL_Rect duck_rect = {game->duck.x, game->duck.y, 100, 100};
	SDL_RenderCopyEx(renderer, game->duck_t, NULL, &duck_rect, 0, NULL, game->duck.flipType);

	// Dibujo imagen bloque: crate
	for (int i=0; i<4; i++) {
		SDL_Rect crate_rect = {game->crates[i].x, game->crates[i].y, 100, 100};
		SDL_RenderCopy(renderer, game->crate, NULL, &crate_rect);
	}

	// Terminamos de dibujar => se presenta lo dibujado
	SDL_RenderPresent(renderer);
}

int main(int argc, const char* argv[]) {
	try {
		if (argc != 3) {
			std::cerr << "Bad program call. Expected " << argv[0] << " <hostname> <servname>.\n";
			return ERROR;
		}

		const std::string hostname = argv[1];
		const std::string servname = argv[2];



		//------------------Clase cliente--------------------------------
		std::cout << "Levanto cliente en hostname: " << hostname << " y servername: " << servname << "\n";

		Client cliente(hostname, servname);
     	cliente.start();	

		//-------------------------------------------------------
		

		/*
		GameState game{};
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;

		SDL_Init(SDL_INIT_VIDEO);  //Inicializo SDL2

		window = SDL_CreateWindow("Duck Game",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									800,600,
									0);
		//window title, initial x position, initial y position, width, height in pixels, flags

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		//window, index, flags

		game.renderer = renderer;

		loadGame(&game);

		// Event Loop: La ventana se abre => se entra al loop
		int done = 0;
		while(!done) {
			//Check de eventos
			done = processEvents(window, &game);
			doRender(renderer, &game);

			//SDL_Delay(20);
		}

		// Shutdown game and free resources
		SDL_DestroyTexture(game.crate);
		SDL_DestroyTexture(game.duck_t);
		SDL_DestroyTexture(game.background);

		// Cerrar y destruir la ventana
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);

		// Limpiar
		SDL_Quit();

		*/
		
		return SUCCESS;

	} catch (const std::exception& err) {
		std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
		return ERROR;
	} catch (...) {
		std::cerr << "Something went wrong and an unknown exception was caught.\n";
		return ERROR;
	}
}
