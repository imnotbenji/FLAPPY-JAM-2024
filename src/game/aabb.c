// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   ______   ______     //
//  /\  __ \ /\  __ \ /\  == \ /\  == \    //
//  \ \  __ \\ \  __ \\ \  __< \ \  __<    //
//   \ \_\ \_\\ \_\ \_\\ \_____\\ \_____\  //
//    \/_/\/_/ \/_/\/_/ \/_____/ \/_____/  //
//                                         //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/aabb.c

#include "game/aabb.h"

bool aabb_check_overlap(float a[4], float b[4])
{
    float a_right = a[0] + a[2];
    float a_top   = a[1] + a[3];
    float b_right = b[0] + b[2];
    float b_top   = b[1] + b[3];

    return (a[0] <= b_right) && (a_right >= b[0]) && (a[1] <= b_top) && (a_top >= b[1]);
}
