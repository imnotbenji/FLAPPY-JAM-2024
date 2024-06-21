// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __    __   ______   ______  __  __   __     //
//  /\ "-./  \ /\  __ \ /\__  _\/\ \_\ \ /\ \    //
//  \ \ \-./\ \\ \  __ \\/_/\ \/\ \  __ \\ \ \   //
//   \ \_\ \ \_\\ \_\ \_\  \ \_\ \ \_\ \_\\ \_\  //
//    \/_/  \/_/ \/_/\/_/   \/_/  \/_/\/_/ \/_/  //
//                                               //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// math/mathi.h

#ifndef MATH_MATHI_H
#define MATH_MATHI_H

#ifdef __cplusplus
extern "C"
{
#endif

    int mathi_sign(int value);

    int mathi_random(int min, int max);

#ifdef __cplusplus
}
#endif

#endif  // MATH_MATHI_H
