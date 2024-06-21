// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __   __   __   __    __   ______   ______  __   ______   __   __     //
//  /\  __ \ /\ "-.\ \ /\ \ /\ "-./  \ /\  __ \ /\__  _\/\ \ /\  __ \ /\ "-.\ \    //
//  \ \  __ \\ \ \-.  \\ \ \\ \ \-./\ \\ \  __ \\/_/\ \/\ \ \\ \ \/\ \\ \ \-.  \   //
//   \ \_\ \_\\ \_\\"\_\\ \_\\ \_\ \ \_\\ \_\ \_\  \ \_\ \ \_\\ \_____\\ \_\\"\_\  //
//    \/_/\/_/ \/_/ \/_/ \/_/ \/_/  \/_/ \/_/\/_/   \/_/  \/_/ \/_____/ \/_/ \/_/  //
//                                                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/animation.c

#include <math.h>

#include "game/animation.h"

void animation_step(animation_t* animation, float dt)
{
    animation->time_elapsed += dt;

    float total_time = animation->time_per_frame * animation->frames_len;

    if (animation->time_elapsed >= total_time)
    {
        animation->loops++;
        animation->time_elapsed = animation->loop ? fmod(animation->time_elapsed, total_time) : animation->time_elapsed;
    }
}

uint8_t animation_get_frame_index(animation_t animation)
{
    float total_time  = animation.time_per_frame * animation.frames_len;
    float looped_time = animation.loop ? fmod(animation.time_elapsed, total_time) : animation.time_elapsed;

    uint8_t frame_index = (uint8_t)(looped_time / animation.time_per_frame);

    if (!animation.loop && frame_index >= animation.frames_len)
    {
        frame_index = animation.frames_len - 1;
    }

    return frame_index;
}
