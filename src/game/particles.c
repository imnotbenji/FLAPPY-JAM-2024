// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______   ______  ______  __   ______   __       ______   ______     //
//  /\  == \/\  __ \ /\  == \/\__  _\/\ \ /\  ___\ /\ \     /\  ___\ /\  ___\    //
//  \ \  _-/\ \  __ \\ \  __<\/_/\ \/\ \ \\ \ \____\ \ \____\ \  __\ \ \___  \   //
//   \ \_\   \ \_\ \_\\ \_\ \_\ \ \_\ \ \_\\ \_____\\ \_____\\ \_____\\/\_____\  //
//    \/_/    \/_/\/_/ \/_/ /_/  \/_/  \/_/ \/_____/ \/_____/ \/_____/ \/_____/  //
//                                                                               //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/particles.c

#include <math.h>
#include <string.h>

#include "game/particles.h"

#include "utils/pool.h"

#include "math/mathi.h"
#include "math/mathf.h"
#include "math/vector.h"

#include "graphics/batch.h"

POOL_DEFINE(particle_t, particles)

void particles_init(void) { particles_pool_new(4096, POOL_FLAGS_RECYCLE); }

void particles_shutdown(void) { particles_pool_delete(); }

void particles_update(double dt, double total)
{
    POOL_LOOP_REVERSE(particle_t, particles, particle)
    {
        particle->time -= dt;

        if (particle->time <= 0)
        {
            size_t index = particle - particles_pool_begin();
            particles_pool_remove(index);
            continue;
        }

        particle->x += particle->speed * sinf(particle->angle);
        particle->y += particle->speed * -cosf(particle->angle);
    }
}

void particles_render(double dt, double total)
{
    POOL_LOOP_FORWARD(particle_t, particles, particle)
    {
        float dst[4] = {

            particle->x - particle->scale / 2,
            particle->y - particle->scale / 2,
            particle->scale,
            particle->scale,
        };

        batch_set_tint(0xffffff, particle->time / particle->total);
        batch_set_rotation(particle->rotation, (float[2]){particle->x + particle->scale / 2, particle->y + particle->scale / 2});
        batch_draw_texture(particle->src, dst);
        batch_set_rotation(0, VEC2_ZERO.xy);
    }
}

static particle_t particle_spawn(particle_system_t system)
{
    float x, y;

    float time  = mathf_random(system.life_min, system.life_max);
    float scale = mathf_random(system.scale_min_start, system.scale_max_start);
    float speed = mathf_random(system.speed_min_start, system.speed_max_start);

    int   spread  = rand() % (system.spread + 1);
    int   degrees = system.direction + spread;
    float angle   = degrees * (PI / 180);

    float rotation = mathi_random(system.rotation_min_start, system.rotation_max_start);

    system.width /= 2;
    system.height /= 2;

    system.x -= system.width;
    system.y -= system.height;

    float r     = system.width * sqrtf((rand()) / (float)(RAND_MAX));
    float theta = 2 * PI * (rand()) / (float)(RAND_MAX);

    x = r * cosf(theta);
    y = system.height / system.width * r * sinf(theta);

    x += system.x;
    y += system.y;

    particle_t particle = {
        .time     = time,
        .total    = time,
        .x        = x,
        .y        = y,
        .speed    = speed,
        .scale    = scale,
        .angle    = angle,
        .rotation = rotation,
    };

    memcpy(particle.src, system.src, 4 * sizeof(float));

    return particle;
}

void particle_system_emit(particle_system_t system)
{
    for (int i = 0; i < system.amount; ++i)
    {
        particle_t particle = particle_spawn(system);

        particles_pool_insert(particle);
    }
}
