#ifndef _SOUND_H_
#define _SOUND_H_

#include "SDL/SDL_mixer.h"

#define FOOD_SOUND  "Resources/sound/food.wav"
#define TREAT_SOUND "Resources/sound/treat.wav"
#define DEATH_SOUND "Resources/sound/death.wav"

void sound_init();

void food_sound();
void treat_sound();
void death_sound();

#endif /* _SOUND_H_ */

