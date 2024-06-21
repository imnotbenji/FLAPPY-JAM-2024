// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______   __  __   __   ______  ______     //
//  /\  ___\/\  == \ /\ \/\ \ /\ \ /\__  _\/\  ___\    //
//  \ \  __\\ \  __< \ \ \_\ \\ \ \\/_/\ \/\ \___  \   //
//   \ \_\   \ \_\ \_\\ \_____\\ \_\  \ \_\ \/\_____\  //
//    \/_/    \/_/ /_/ \/_____/ \/_/   \/_/  \/_____/  //
//                                                     //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/fruits.h

#ifndef GAME_FRUITS_H
#define GAME_FRUITS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        float x;
        float y;

        float scale;
        bool  active;
    } fruit_t;

    void fruits_init(void);
    void fruits_shutdown(void);

    void fruits_update(float dt, float total);
    void fruits_render(float dt, float total);

    void fruits_spawn(void);

#ifdef __cplusplus
}
#endif

#endif  // GAME_FRUITS_H
