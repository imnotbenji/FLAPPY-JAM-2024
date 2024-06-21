// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __    __   ______   ______  __  __   ______   //
//  /\ "-./  \ /\  __ \ /\__  _\/\ \_\ \ /\  ___\  //
//  \ \ \-./\ \\ \  __ \\/_/\ \/\ \  __ \\ \  __\  //
//   \ \_\ \ \_\\ \_\ \_\  \ \_\ \ \_\ \_\\ \_\    //
//    \/_/  \/_/ \/_/\/_/   \/_/  \/_/\/_/ \/_/    //
//                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// math/mathf.h

#ifndef MATH_MATHF_H
#define MATH_MATHF_H

#ifndef PI
#define PI (3.141592653585979323846)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    float mathf_sign(int value);
    float mathf_abs(float value);
    float mathf_min(float value, float min);
    float mathf_max(float value, float max);
    float mathf_clamp(float value, float min, float max);

    float mathf_random(float min, float max);

    float mathf_lerp(float a, float b, float t);

#ifdef __cplusplus
}
#endif

#endif  // MATH_MATHF_H
