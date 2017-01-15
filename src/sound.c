#include "sound.h"

static Mix_Chunk *food;
static Mix_Chunk *treat;
static Mix_Chunk *death;

void sound_quit()
{
    Mix_FreeChunk(food);
    Mix_FreeChunk(treat);
    Mix_FreeChunk(death);
    Mix_CloseAudio();
}

void sound_init()
{
    if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 8192 ) < 0) {
        fprintf(stderr, "Could not initialize SDL_mixer\n");
        exit(1);
    }

    food = Mix_LoadWAV(FOOD_SOUND);
    treat = Mix_LoadWAV(TREAT_SOUND);
    death = Mix_LoadWAV(DEATH_SOUND);

    if (food == NULL || treat == NULL || death == NULL) {
        fprintf(stderr, "Error loading sound files\n");
    }

    atexit(sound_quit);
}

void food_sound()
{
    Mix_PlayChannel(-1, food, 0);
}

void treat_sound()
{
    Mix_PlayChannel(-1, treat, 0);
}

void death_sound()
{
    Mix_PlayChannel(-1, death, 0);
}

