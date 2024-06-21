// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __   __   __   __    __   ______   ______  __   ______   __   __     //
//  /\  __ \ /\ "-.\ \ /\ \ /\ "-./  \ /\  __ \ /\__  _\/\ \ /\  __ \ /\ "-.\ \    //
//  \ \  __ \\ \ \-.  \\ \ \\ \ \-./\ \\ \  __ \\/_/\ \/\ \ \\ \ \/\ \\ \ \-.  \   //
//   \ \_\ \_\\ \_\\"\_\\ \_\\ \_\ \ \_\\ \_\ \_\  \ \_\ \ \_\\ \_____\\ \_\\"\_\  //
//    \/_/\/_/ \/_/ \/_/ \/_/ \/_/  \/_/ \/_/\/_/   \/_/  \/_/ \/_____/ \/_/ \/_/  //
//                                                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/animation.h

#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

#define ANIMATION2D_MAX_FRAMES (255)

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int     frames[ANIMATION2D_MAX_FRAMES][4];
        uint8_t frames_len;

        bool    loop;
        uint8_t loops;

        float time_elapsed;
        float time_per_frame;
    } animation_t;

    void    animation_step(animation_t* animation, float dt);
    uint8_t animation_get_frame_index(animation_t animation);

#ifdef __cplusplus
}
#endif

#endif  // GAME_ANIMATION_H
