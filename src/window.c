#include "window.h"

static SDL_Surface *window;
static size_t window_width, window_height;

#define BLACK SDL_MapRGB(window->format, 0x00, 0x00, 0x00)
#define WHITE SDL_MapRGB(window->format, 0xFF, 0xFF, 0xFF)
#define RED   SDL_MapRGB(window->format, 0xFF, 0x00, 0x00)

void draw_field(Game *game)
{
    char *score = calloc(50, sizeof(char));
    sprintf(score, "%s — score: %d", TITLE, game->score);
    SDL_WM_SetCaption(score, NULL);
    free(score);

    for (int i = 0; i < game->width; ++i) {
        for (int j = 0; j < game->height; ++j) {
            SDL_Rect block = { 1 + i * BLOCK_SIZE
                             , 1 + j * BLOCK_SIZE
                             , BLOCK_SIZE - 2
                             , BLOCK_SIZE - 2
                             };
            Uint32 color;
            switch (game->field[i][j]) {
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
    HighScore *hs = get_scores();

    SDL_FillRect(window, NULL, BLACK);

    TTF_Font *font = TTF_OpenFont(FONTFILE, BLOCK_SIZE * 2 / 3);
    if (font == NULL) {
        fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Color black = { 0, 0, 0 };
    SDL_Surface *score_fg;
    char *score = calloc(50, sizeof(char));
    sprintf(score, "   Highscores   ");
    int text_width;
    TTF_SizeText(font, score, &text_width, NULL);
    int text_x_position = window_width / 2 - text_width / 2;
    score_fg = TTF_RenderText_Blended(font, score, black);
    SDL_Rect score_bg = { text_x_position
                        , 1
                        , text_width
                        , BLOCK_SIZE - 2
                        };
    SDL_FillRect(window, &score_bg, WHITE);
    SDL_BlitSurface(score_fg, NULL, window, &score_bg);

    for (int i = 0; i < NR_OF_SCORES; ++i) {
        sprintf(score, " <%02d> %5d %s ", i + 1, hs[i].score, hs[i].name);
        score_fg = TTF_RenderText_Blended(font, score, black);
        SDL_Rect score_bg = { text_x_position
                            , 1 + (i + 1) * BLOCK_SIZE
                            , text_width
                            , BLOCK_SIZE - 2
                            };
        SDL_FillRect(window, &score_bg, WHITE);
        SDL_BlitSurface(score_fg, NULL, window, &score_bg);
    }

    free(score);
    free(hs);
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
    TTF_Font *font = TTF_OpenFont(FONTFILE, BLOCK_SIZE * 2 / 3);
    if (font == NULL) {
        fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
        exit(1);
    }

    char *name = calloc(4, sizeof(char));
    strcpy(name, "___");
    char *box = calloc(12, sizeof(char));
    sprintf(box, " NAME: %s ", name);

    int box_width;
    TTF_SizeText(font, box, &box_width, NULL);

    SDL_Color font_color = { 0xFF, 0xFF, 0xFF };
    SDL_Rect bg = { window_width / 2 - box_width / 2
                  , window_height / 2
                  , box_width
                  , BLOCK_SIZE - 2
                  };

    SDL_Event event;
    int i = 0, entering_text = 1;
    while (entering_text) {
        sprintf(box, " NAME: %s ", name);
        SDL_Surface *fg = TTF_RenderText_Blended(font, box, font_color);
        SDL_FillRect(window, &bg, BLACK);
        SDL_BlitSurface(fg, NULL, window, &bg);
        SDL_FreeSurface(fg);
        SDL_Flip(window);
        SDL_Delay(16);

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
        }
    }

    free(box);
    TTF_CloseFont(font);
    return name;
}

void window_quit()
{
    puts("freeing window");
    SDL_FreeSurface(window);
    puts("quitting TTF");
    TTF_Quit();
    puts("quitting SDL");
    SDL_Quit();
}

void window_init(size_t field_width, size_t field_height)
{
    window_width = field_width * BLOCK_SIZE;
    window_height = field_height * BLOCK_SIZE;

    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_SetVideoMode(window_width, window_height, 0,
                              SDL_HWPALETTE | SDL_DOUBLEBUF);
    if (window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "Could not initialize SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    SDL_WM_SetCaption(TITLE, NULL);
    clear_screen();
}

void window_resize(size_t field_width, size_t field_height)
{
    SDL_Quit();
    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    window_width = field_width * BLOCK_SIZE;
    window_height = field_height * BLOCK_SIZE; 
    puts("new window");
    window = SDL_SetVideoMode(window_width, window_height, 0,
                              SDL_HWPALETTE | SDL_DOUBLEBUF);
    clear_screen();
}
