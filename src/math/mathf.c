// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __    __   ______   ______  __  __   ______   //
//  /\ "-./  \ /\  __ \ /\__  _\/\ \_\ \ /\  ___\  //
//  \ \ \-./\ \\ \  __ \\/_/\ \/\ \  __ \\ \  __\  //
//   \ \_\ \ \_\\ \_\ \_\  \ \_\ \ \_\ \_\\ \_\    //
//    \/_/  \/_/ \/_/\/_/   \/_/  \/_/\/_/ \/_/    //
//                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// math/mathf.c

#include <math.h>
#include <stdlib.h>

#include "math/mathf.h"

float mathf_sign(int value) { return (value > 0) - (value < 0); }

float mathf_abs(float value) { return fabs(value); }

float mathf_min(float value, float min) { return value > min ? min : value; }

float mathf_max(float value, float max) { return value < max ? max : value; }

float mathf_clamp(float value, float min, float max)
{
    float t = value < min ? min : value;
    return t > max ? max : t;
}

float mathf_random(float min, float max) { return (min + 1) + ((rand()) / (float)RAND_MAX) * (max - (min + 1)); }

float mathf_lerp(float a, float b, float t) { return (1 - t) * a + t * b; }
