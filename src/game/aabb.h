// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   ______   ______     //
//  /\  __ \ /\  __ \ /\  == \ /\  == \    //
//  \ \  __ \\ \  __ \\ \  __< \ \  __<    //
//   \ \_\ \_\\ \_\ \_\\ \_____\\ \_____\  //
//    \/_/\/_/ \/_/\/_/ \/_____/ \/_____/  //
//                                         //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/aabb.h

#ifndef GAME_AABB_H
#define GAME_AABB_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool aabb_check_overlap(float a[4], float b[4]);

#ifdef __cplusplus
}
#endif

#endif  // GAME_AABB_H
