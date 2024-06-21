// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __    __   ______   ______   ______     //
//  /\  ___\ /\  __ \ /\ "-./  \ /\  ___\ /\  == \ /\  __ \    //
//  \ \ \____\ \  __ \\ \ \-./\ \\ \  __\ \ \  __< \ \  __ \   //
//   \ \_____\\ \_\ \_\\ \_\ \ \_\\ \_____\\ \_\ \_\\ \_\ \_\  //
//    \/_____/ \/_/\/_/ \/_/  \/_/ \/_____/ \/_/ /_/ \/_/\/_/  //
//                                                             //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/camera.h

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void camera_init(void);

    void camera_update(float dt, float total);

    void camera_set_zoom(float scale);
    void camera_set_position(float x, float y);

    void camera_get_zoom(float* zoom);
    void camera_get_position(float* x, float* y);
    void camera_get_matrix(float out[4][4]);

    void camera_shake(float time, float amount);

    bool camera_within_bounds(float y, float height);

#ifdef __cplusplus
}
#endif

#endif  // GAME_CAMERA_H
