// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______   ______  ______  __   ______   __       ______   ______     //
//  /\  == \/\  __ \ /\  == \/\__  _\/\ \ /\  ___\ /\ \     /\  ___\ /\  ___\    //
//  \ \  _-/\ \  __ \\ \  __<\/_/\ \/\ \ \\ \ \____\ \ \____\ \  __\ \ \___  \   //
//   \ \_\   \ \_\ \_\\ \_\ \_\ \ \_\ \ \_\\ \_____\\ \_____\\ \_____\\/\_____\  //
//    \/_/    \/_/\/_/ \/_/ /_/  \/_/  \/_/ \/_____/ \/_____/ \/_____/ \/_____/  //
//                                                                               //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/particles.h

#ifndef GAME_PARTICLES_H
#define GAME_PARTICLES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    enum
    {
        PARTICLE_SYSTEM_FLAGS_NONE = 0x0,
        PARTICLE_SYSTEM_FLAGS_LOOP = 1 << 0,
        PARTICLE_SYSTEM_FLAGS_WARM = 1 << 1,
    };

    typedef struct
    {
        float time;
        float total;

        float x;
        float y;
        float scale;
        float speed;
        float angle;
        float rotation;

        float src[4];
    } particle_t;

    typedef struct
    {
        uint32_t flags;

        float x;
        float y;
        float width;
        float height;

        int amount;

        int spread;
        int direction;

        float life_min;
        float life_max;

        float speed_min_start;
        float speed_max_start;

        float scale_min_start;
        float scale_max_start;

        int rotation_min_start;
        int rotation_max_start;

        float src[4];
    } particle_system_t;

    void particles_init(void);
    void particles_shutdown(void);

    void particles_update(double dt, double total);
    void particles_render(double dt, double total);

    void particle_system_emit(particle_system_t system);

#ifdef __cplusplus
}
#endif

#endif  // GAME_PARTICLES_H
