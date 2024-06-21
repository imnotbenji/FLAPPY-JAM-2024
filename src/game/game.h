// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __    __   ______     //
//  /\  ___\ /\  __ \ /\ "-./  \ /\  ___\    //
//  \ \ \__ \\ \  __ \\ \ \-./\ \\ \  __\    //
//   \ \_____\\ \_\ \_\\ \_\ \ \_\\ \_____\  //
//    \/_____/ \/_/\/_/ \/_/  \/_/ \/_____/  //
//                                           //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/game.h

#ifndef GAME_H
#define GAME_H

#define GAME_WIDTH  (960)
#define GAME_HEIGHT (1280)

#ifdef __cplusplus
extern "C"
{
#endif

    void game_init(void);
    void game_shutdown(void);

    void game_update(double dt, double total);
    void game_render(double dt, double total);

    void game_sleep(float ms);

#ifdef __cplusplus
}
#endif

#endif  // GAME_H
