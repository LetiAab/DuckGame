#ifndef SDL_BUTTON_H
#define SDL_BUTTON_H
#include <SDL_render.h>

class Button {

public:
    SDL_Texture* texture;
    SDL_Rect srect, drect;
    bool isSelected = false;
    Button();
    virtual ~Button();
    //void update(Mouse& mouse);
};



#endif //SDL_BUTTON_H
