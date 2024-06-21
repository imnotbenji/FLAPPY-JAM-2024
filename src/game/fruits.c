// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______   __  __   __   ______  ______     //
//  /\  ___\/\  == \ /\ \/\ \ /\ \ /\__  _\/\  ___\    //
//  \ \  __\\ \  __< \ \ \_\ \\ \ \\/_/\ \/\ \___  \   //
//   \ \_\   \ \_\ \_\\ \_____\\ \_\  \ \_\ \/\_____\  //
//    \/_/    \/_/ /_/ \/_____/ \/_/   \/_/  \/_____/  //
//                                                     //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/fruits.c

#include <math.h>

#include "game/fruits.h"
#include "game/aabb.h"
#include "game/game.h"
#include "game/camera.h"
#include "game/player.h"
#include "game/particles.h"

#include "platform/content.h"

#include "utils/pool.h"

#include "math/mathi.h"
#include "math/vector.h"

#include "graphics/batch.h"

#define FRUITS_NUM      (128)

#define FRUIT_SIZE      (64)
#define FRUIT_SIZE_HALF (FRUIT_SIZE / 2.0)

POOL_DEFINE(fruit_t, fruits)

static float fruit_texture_src[4];

static particle_system_t sparkles;

static void fruit_get_center(fruit_t fruit, float* x, float* y);
static void fruit_get_rect(fruit_t fruit, float rect[4]);

void fruits_init(void)
{
    fruits_pool_new(FRUITS_NUM, POOL_FLAGS_ASSERT);

    sparkles = (particle_system_t){
        .width  = FRUIT_SIZE,
        .height = FRUIT_SIZE,
        .amount = 3,

        .spread    = 360,
        .direction = 0,

        .life_min = 0.4,
        .life_max = 1.4,

        .speed_min_start = 0,
        .speed_max_start = 2,

        .scale_min_start = 12,
        .scale_max_start = 21,
    };

    int(*texture)[4];

    content_find_textures("fruit1", &texture);

    fruit_texture_src[0] = (*texture)[0];
    fruit_texture_src[1] = (*texture)[1];
    fruit_texture_src[2] = (*texture)[2];
    fruit_texture_src[3] = (*texture)[3];

    content_find_textures("sparkle", &texture);

    sparkles.src[0] = (*texture)[0];
    sparkles.src[1] = (*texture)[1];
    sparkles.src[2] = (*texture)[2];
    sparkles.src[3] = (*texture)[3];
}

void fruits_shutdown(void) { fruits_pool_delete(); }

static void fruit_get_center(fruit_t fruit, float* x, float* y)
{
    *x = fruit.x + FRUIT_SIZE_HALF;
    *y = fruit.y + FRUIT_SIZE_HALF;
}

static void fruit_get_rect(fruit_t fruit, float rect[4])
{
    rect[0] = fruit.x;
    rect[1] = fruit.y;
    rect[2] = FRUIT_SIZE;
    rect[3] = FRUIT_SIZE;
}

void fruits_update(float dt, float total)
{
    static float fruit_rect[4];
    static float player_rect[4];

    player_get_rect(player_rect);

    POOL_LOOP_FORWARD(fruit_t, fruits, fruit)
    {
        fruit_get_rect(*fruit, fruit_rect);

        if (camera_within_bounds(fruit_rect[1], fruit_rect[3]))
        {
            if (!fruit->active)
            {
                fruit->scale -= 8 * dt;
                continue;
            }

            if (aabb_check_overlap(player_rect, fruit_rect))
            {
                fruit->active = false;
                fruit->scale  = 1.3;

                fruit_get_center(*fruit, &sparkles.x, &sparkles.y);

                particle_system_emit(sparkles);
                camera_shake(0.1, 2);
                game_sleep(0.05);

                player_add_star();
            }
        }
    }

    for (int i = fruits_pool.count - 1; i >= 0; --i)
    {
        fruit_t* fruit = &fruits_pool.data[i];

        if (fruit->scale <= 0)
        {
            fruits_pool_remove(i);
        }
    }
}

void fruits_render(float dt, float total)
{
    static float dst[4];
    static float rect[4];

    batch_set_tint(0xffffff, 1);

    POOL_LOOP_FORWARD(fruit_t, fruits, fruit)
    {
        fruit_get_rect(*fruit, rect);

        if (camera_within_bounds(rect[1], rect[3]))
        {
            float size_half_scaled = FRUIT_SIZE_HALF * (1.0 - fruit->scale);

            dst[0] = rect[0] + size_half_scaled;
            dst[1] = rect[1] + size_half_scaled;
            dst[2] = rect[2] * fruit->scale;
            dst[3] = rect[3] * fruit->scale;

            batch_set_rotation(
                sinf((total + (fruit->y + fruit->x)) * 2) * 30,
                (float[2]){
                    dst[0] + FRUIT_SIZE_HALF - size_half_scaled,
                    dst[1] + FRUIT_SIZE_HALF - size_half_scaled,
                }
            );
            batch_set_fill(0xffffff, !fruit->active);
            batch_draw_texture(fruit_texture_src, dst);
            batch_set_rotation(0, VEC2_ZERO.xy);
        }
    }
}

void fruits_spawn(void)
{
    fruits_pool.count = 0;

    for (int i = 0; i < FRUITS_NUM; ++i)
    {
        float x = mathi_random(FRUIT_SIZE, 960 - FRUIT_SIZE);
        float y = -960 - 256 + i * -256 - mathi_random(0, 1) * 256;

        fruit_t fruit = {
            .x      = x,
            .y      = y,
            .scale  = 1,
            .active = true,
        };

        fruits_pool_insert(fruit);
    }
}
