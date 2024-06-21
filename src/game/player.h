// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  __       ______   __  __   ______   ______     //
//  /\  == \/\ \     /\  __ \ /\ \_\ \ /\  ___\ /\  == \    //
//  \ \  _-/\ \ \____\ \  __ \\ \____ \\ \  __\ \ \  __<    //
//   \ \_\   \ \_____\\ \_\ \_\\/\_____\\ \_____\\ \_\ \_\  //
//    \/_/    \/_____/ \/_/\/_/ \/_____/ \/_____/ \/_/ /_/  //
//                                                          //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/player.h

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void player_init(void);

    void player_update(float dt, float total);
    void player_render(float dt, float total);

    void player_get_position(float* x, float* y);
    void player_get_rect(float rect[4]);

    void player_add_star(void);

#ifdef __cplusplus
}
#endif

#endif  // GAME_PLAYER_H
