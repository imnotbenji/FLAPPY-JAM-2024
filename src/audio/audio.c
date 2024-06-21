// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __  __   _____    __   ______     //
//  /\  __ \ /\ \/\ \ /\  __-. /\ \ /\  __ \    //
//  \ \  __ \\ \ \_\ \\ \ \/\ \\ \ \\ \ \/\ \   //
//   \ \_\ \_\\ \_____\\ \____- \ \_\\ \_____\  //
//    \/_/\/_/ \/_____/ \/____/  \/_/ \/_____/  //
//                                              //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// audio/audio.c

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define CUTE_SOUND_FORCE_SDL
#define CUTE_SOUND_IMPLEMENTATION
#define CUTE_SOUND_SCALAR_MODE
#define CUTE_SOUND_SDL_H "SDL.h"
#include "cute_sound.h"

#include "audio/audio.h"

void audio_init(void)
{
    cs_error_t error = cs_init(NULL, 44100, 1024, NULL);
    assert(error == CUTE_SOUND_ERROR_NONE);
}

void audio_shutdown(void) { cs_shutdown(); }

void audio_update(float dt) { cs_update(dt); }
