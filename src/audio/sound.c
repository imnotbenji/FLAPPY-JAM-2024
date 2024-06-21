// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __  __   __   __   _____     //
//  /\  ___\ /\  __ \ /\ \/\ \ /\ "-.\ \ /\  __-.   //
//  \ \___  \\ \ \/\ \\ \ \_\ \\ \ \-.  \\ \ \/\ \  //
//   \/\_____\\ \_____\\ \_____\\ \_\\"\_\\ \____-  //
//    \/_____/ \/_____/ \/_____/ \/_/ \/_/ \/____/  //
//                                                  //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// audio/sounds.c

#include <assert.h>
#include <stdlib.h>

#include "cute_sound.h"

#include "audio/sound.h"

struct sound_t
{
    cs_audio_source_t* source;
};

sound_t* sound_new(const char* filepath)
{
    sound_t* sound = (sound_t*)malloc(sizeof(sound_t));

    assert(sound);

    cs_error_t         error;
    cs_audio_source_t* source = cs_load_wav(filepath, &error);

    assert(error == CUTE_SOUND_ERROR_NONE);

    sound->source = source;

    return sound;
}

void sound_delete(sound_t* sound) { cs_free_audio_source(sound->source); }

sound_ref_t sound_play(sound_t* sound, float volume, bool loop)
{
    cs_sound_params_t csparams = cs_sound_params_default();

    csparams.volume = volume;
    csparams.looped = loop;

    return cs_play_sound(sound->source, csparams).id;
}

void sound_set_volume(sound_ref_t id, float volume)
{
    cs_playing_sound_t cssound = (cs_playing_sound_t){.id = id};
    cs_sound_set_volume(cssound, volume);
}
