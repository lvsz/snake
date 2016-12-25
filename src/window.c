#include "window.h"

static SDL_Surface *window;

#define BLACK SDL_MapRGB(window->format, 0, 0, 0)
#define WHITE SDL_MapRGB(window->format, 255, 255, 255)
#define RED SDL_MapRGB(window->format, 255, 0, 0)

void draw_field(char **field)
{
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            SDL_Rect block = { 1 + i * 40, 1 + j * 40, 38, 38 };
            Uint32 color;
            switch (field[i][j]) {
                case 's':
                    SDL_FillRect(window, &block, BLACK);
                    break;
                case 'f':
                    SDL_FillRect(window, &block, RED);
                    break;
                default:
                    SDL_FillRect(window, &block, WHITE);
                    break;
            }
        }
    }
    SDL_Flip(window);
}

void quit()
{
    SDL_FreeSurface(window);
    SDL_Quit();
}

void window_init(char *title)
{
    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                              SDL_HWPALETTE | SDL_DOUBLEBUF);
    if (window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption(title, NULL);
    SDL_FillRect(window, NULL, 0xFFFFFF00);

    atexit(quit);
}
