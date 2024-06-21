// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __  __   _____    __   ______     //
//  /\  __ \ /\ \/\ \ /\  __-. /\ \ /\  __ \    //
//  \ \  __ \\ \ \_\ \\ \ \/\ \\ \ \\ \ \/\ \   //
//   \ \_\ \_\\ \_____\\ \____- \ \_\\ \_____\  //
//    \/_/\/_/ \/_____/ \/____/  \/_/ \/_____/  //
//                                              //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// audio/audio.h

#ifndef AUDIO_AUDIO_H
#define AUDIO_AUDIO_H

#ifdef __cplusplus
extern "C"
{
#endif

    void audio_init(void);
    void audio_shutdown(void);

    void audio_update(float dt);

#ifdef __cplusplus
}
#endif

#endif  // AUDIO_AUDIO_H
