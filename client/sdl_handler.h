#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#include <SDL2/SDL.h>

#include "common/message.h"
#include "common/queue.h"

struct Duck {
    int x, y;
    SDL_RendererFlip flipType;
};

struct Crate {
    int x, y;
};

struct GameState {
    Duck duck;
    std::vector<Crate> crates;
    SDL_Texture *crate, *duck_t, *background;
    SDL_Renderer* renderer;
    std::vector<std::vector<int>> *map;
    Queue<Message>* message_queue;
};

class SDLHandler {
public:
    SDLHandler();
    ~SDLHandler();
    void run(Queue<Message>& message_queue);

private:
    SDL_Surface* loadImage(const std::string& name_img);
    void loadGame(GameState* game);
    //void locateObject(GameState* game, int& num);
    int processEvents(SDL_Window* window, GameState* game, uint16_t id);
    void doRender(SDL_Renderer* renderer, GameState* game);
};

#endif //SDL_HANDLER_H
