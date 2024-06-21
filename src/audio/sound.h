// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __  __   __   __   _____     //
//  /\  ___\ /\  __ \ /\ \/\ \ /\ "-.\ \ /\  __-.   //
//  \ \___  \\ \ \/\ \\ \ \_\ \\ \ \-.  \\ \ \/\ \  //
//   \/\_____\\ \_____\\ \_____\\ \_\\"\_\\ \____-  //
//    \/_____/ \/_____/ \/_____/ \/_/ \/_/ \/____/  //
//                                                  //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// audio/sound.h

#ifndef AUDIO_SOUND_H
#define AUDIO_SOUND_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct sound_t sound_t;
    typedef uint64_t       sound_ref_t;

    sound_t* sound_new(const char* filepath);
    void     sound_delete(sound_t* sound);

    sound_ref_t sound_play(sound_t* sound, float volume, bool loop);

    void sound_set_volume(sound_ref_t id, float volume);

#ifdef __cplusplus
}
#endif

#endif  // AUDIO_SOUND_H
