#include "input.h"

Input read_input()
{
    SDL_Event event;

    int mouse_x, mouse_y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return QUIT;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        return P1_UP;
                    case SDLK_DOWN:
                        return P1_DOWN;
                    case SDLK_LEFT:
                        return P1_LEFT;
                    case SDLK_RIGHT:
                        return P1_RIGHT;
                    case SDLK_ESCAPE:
                    case SDLK_p:
                        return PAUSE;
                    case SDLK_s:
                        return SAVE;
                    default:
                        return NOTHING;
                }
            case SDL_MOUSEBUTTONDOWN:
                puts("mouseclick");
                break;
        }
    }
    return NOTHING;
}

