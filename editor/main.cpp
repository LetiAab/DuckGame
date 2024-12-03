#include "../common/foo.h"
#include <iostream>

#include "level_editor.h"
#include <SDL2/SDL.h>

int main() {
    LevelEditor editor;
    if (!editor.init()) {
        return -1;
    }
    editor.run();
    editor.cleanup();
    return 0;
}