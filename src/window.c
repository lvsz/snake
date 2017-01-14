#include "window.h"

static SDL_Surface *window;
static size_t window_width, window_height;

static Mix_Chunk *sound;
#define TITLEBUFFER 40

#define BLACK SDL_MapRGB(window->format, 0x00, 0x00, 0x00)
#define WHITE SDL_MapRGB(window->format, 0xFF, 0xFF, 0xFF)
#define GREEN SDL_MapRGB(window->format, 0x00, 0xBB, 0x00)
#define BLUE  SDL_MapRGB(window->format, 0x00, 0xAA, 0xDD)
#define GOLD  SDL_MapRGB(window->format, 0xFF, 0xCC, 0x00)
#define RED   SDL_MapRGB(window->format, 0xFF, 0x00, 0x00)

void draw_field(Game *game)
{
    char title[TITLEBUFFER];
    if (game->players > 1) {
        sprintf(title, "%s — multiplayer", TITLE);
    } else if (game->level > 0) {
        sprintf(title, "%s — level %d — score: %d/%d",
                TITLE,
                game->level,
                game->total_score + game->p1->score,
                game->total_score + TARGET_SCORE);
    } else {
        sprintf(title, "%s — score: %d",
                TITLE, game->p1->score);
    }

    SDL_WM_SetCaption(title, NULL);

    for (int i = 0; i < game->width; ++i) {
        for (int j = 0; j < game->height; ++j) {
            SDL_Rect block = { 1 + i * BLOCK_SIZE
                             , 1 + j * BLOCK_SIZE
                             , BLOCK_SIZE - 2
                             , BLOCK_SIZE - 2
                             };
            Uint32 color;
            switch (game->field[i][j]) {
                case '1':
                    SDL_FillRect(window, &block, GREEN);
                    break;
                case '2':
                    SDL_FillRect(window, &block, BLUE);
                    break;
                case 'f':
                    SDL_FillRect(window, &block, RED);
                    break;
                case 't':
                    SDL_FillRect(window, &block, GOLD);
                    break;
                case 'w':
                    SDL_FillRect(window, &block, BLACK);
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

    SDL_Color black = { 0, 0, 0 };
    SDL_Surface *score_fg;
    char score[] =  "   Highscores   ";
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
        SDL_Delay(20);

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
            } else if (event.type == SDL_QUIT) {
                free(name);
                name = NULL;
                entering_text = 0;
            }
        }
    }

    free(box);
    TTF_CloseFont(font);
    return name;
}

int show_winner(int player_nr)
{
    TTF_Font *font = TTF_OpenFont(FONTFILE, BLOCK_SIZE * 2 / 3);

    char winner[sizeof(" IT'S A TIE! ")];
    if (player_nr == 1) {
        strcpy(winner, " GREEN WINS! ");
    } else if (player_nr == 2) {
        strcpy(winner, " BLUE WINS! ");
    } else {
        strcpy(winner, " IT'S A TIE! ");
    }

    int text_width;
    TTF_SizeText(font, winner, &text_width, NULL);

    SDL_Color font_color = { 0xFF, 0xFF, 0xFF };
    SDL_Rect bg = { window_width / 2 - text_width / 2
                  , window_height / 2
                  , text_width
                  , BLOCK_SIZE - 2
                  };
    SDL_Surface *fg = TTF_RenderText_Blended(font, winner, font_color);

    SDL_FillRect(window, &bg, BLACK);
    SDL_BlitSurface(fg, NULL, window, &bg);
    SDL_Flip(window);

    SDL_FreeSurface(fg);
    TTF_CloseFont(font);

    SDL_Event event;
    while (1) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return 0;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                        case SDLK_RETURN:
                        case SDLK_ESCAPE:
                            return 1;
                        default:
                            break;
                    }
                default:
                    SDL_Delay(20);
            }
        }
    }
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

    atexit(window_quit);
}

void window_resize(size_t field_width, size_t field_height)
{
    SDL_FreeSurface(window);

    window_width = field_width * BLOCK_SIZE;
    window_height = field_height * BLOCK_SIZE;
    window = SDL_SetVideoMode(window_width, window_height, 0,
                              SDL_HWPALETTE | SDL_DOUBLEBUF);
}

void window_pause()
{
    char title[TITLEBUFFER];
    sprintf(title, "%s — Paused", TITLE);
    SDL_WM_SetCaption(title, NULL);
}

void delay(int n)
{
    SDL_Delay(n);
}

