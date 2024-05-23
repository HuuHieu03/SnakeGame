#ifndef BUILDDIR_H
#define BUILDDIR_H

#include "SDL.h"
#include "Graphics.h"


struct Head_dir(int dir) {
    SDL_Texture* Snake_head;
    if (dir == DOWN) {
            Snake_head = graphics.loadTexture("images/snake2_D.png");
        }
    else if (dir == UP) {
            Snake_head = graphics.loadTexture("images/snake2_U.png");
        }
    else if (dir == RIGHT) {
            Snake_head = graphics.loadTexture("images/snake2_R.png");
        }
    else if (dir == LEFT) {
            Snake_head = graphics.loadTexture("images/snake2_L.png");
        }
};

#endif // BUILDDIR_H
