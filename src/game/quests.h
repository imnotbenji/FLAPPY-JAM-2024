// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __  __   ______   ______   ______  ______     //
//  /\  __ \ /\ \/\ \ /\  ___\ /\  ___\ /\__  _\/\  ___\    //
//  \ \ \/\_\\ \ \_\ \\ \  __\ \ \___  \\/_/\ \/\ \___  \   //
//   \ \___\_\\ \_____\\ \_____\\/\_____\  \ \_\ \/\_____\  //
//    \/___/_/ \/_____/ \/_____/ \/_____/   \/_/  \/_____/  //
//                                                          //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/quests.h

#ifndef GAME_QUESTS_H
#define GAME_QUESTS_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void quests_init(void);

    void quests_update(float dt, float total);
    void quests_render(float dt, float total);

#ifdef __cplusplus
}
#endif

#endif  // GAME_QUESTS_H
