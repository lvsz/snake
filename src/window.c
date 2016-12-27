#include "window.h"

static SDL_Surface *window;

#define BLACK SDL_MapRGB(window->format, 0x00, 0x00, 0x00)
#define WHITE SDL_MapRGB(window->format, 0xFF, 0xFF, 0xFF)
#define RED   SDL_MapRGB(window->format, 0xFF, 0x00, 0x00)

void draw_field(char **field)
{
    for (int i = 0; i < FIELD_WIDTH; ++i) {
        for (int j = 0; j < FIELD_HEIGHT; ++j) {
            SDL_Rect block = { 1 + i * BLOCK_SIZE
                             , 1 + j * BLOCK_SIZE
                             , BLOCK_SIZE - 2
                             , BLOCK_SIZE - 2
                             };
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

void clear_screen()
{
    SDL_FillRect(window, NULL, WHITE);
}

int score_screen()
{
    HighScore *s = get_scores();

    SDL_FillRect(window, NULL, BLACK);

    TTF_Font *font = TTF_OpenFont(FONTFILE, 28);
    if (font == NULL) {
        fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Color black = { 0, 0, 0 };
    SDL_Surface *score_fg;

    for (int i = 0; i < NR_OF_SCORES; ++i) {
        char *score = calloc(50, sizeof(char));
        sprintf(score, "   <%02d> %9d %s", i + 1, s[i].score, s[i].name);
        score_fg = TTF_RenderText_Blended(font, score, black);
        SDL_Rect score_bg = { WINDOW_WIDTH / 4
                            , 1 + (i + 1) * BLOCK_SIZE
                            , WINDOW_WIDTH / 2
                            , BLOCK_SIZE - 2
                            };
        SDL_FillRect(window, &score_bg, WHITE);
        SDL_BlitSurface(score_fg, NULL, window, &score_bg);
        free(score);
    }

    free(s);
    TTF_CloseFont(font);
    SDL_FreeSurface(score_fg);

    SDL_Flip(window);

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_SPACE:
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        return 1;
                    default:
                        break;
                }
            }
        }
    }

}

char *get_name()
{
    TTF_Font *font = TTF_OpenFont(FONTFILE, 28);
    if (font == NULL) {
        fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
        exit(1);
    }

    char *name = calloc(4, sizeof(char));
    strcpy(name, "___");

    SDL_Color font_color = { 0xff, 0xff, 0xff };
    SDL_Rect bg = { WINDOW_WIDTH / 2 - BLOCK_SIZE * 2
                  , WINDOW_HEIGHT / 2 - BLOCK_SIZE
                  , BLOCK_SIZE * 4 - 2
                  , BLOCK_SIZE
                  };

    SDL_Event event;
    char *box = calloc(50, sizeof(char));
    int i = 0, entering_text = 1;
    while (entering_text) {
        sprintf(box, "NAME: %s", name);
        SDL_Surface *fg = TTF_RenderText_Blended(font, box, font_color);
        SDL_FillRect(window, &bg, BLACK);
        SDL_BlitSurface(fg, NULL, window, &bg);
        SDL_FreeSurface(fg);
        SDL_Flip(window);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                int key = event.key.keysym.sym;
                if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                    entering_text = 0;
                } else if (key == SDLK_BACKSPACE && i > 0) {
                    name[--i] = '_';
                } else if (key >= SDLK_a && key <= SDLK_z && i < 3) {
                    name[i++] = (char) key ^ 32;
                } else if (key >= SDLK_0 && key <= SDLK_9 && i < 3) {
                    name[i++] = (char) key;
                }
            }
            SDL_Delay(16);
        }
    }

    free(box);
    return name;
}

void quit()
{
    SDL_FreeSurface(window);
    TTF_Quit();
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

    if (TTF_Init() < 0) {
        fprintf(stderr, "Could not initialize SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_WM_SetCaption(title, NULL);
    clear_screen();

    atexit(quit);
}

