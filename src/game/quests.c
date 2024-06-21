// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __  __   ______   ______   ______  ______     //
//  /\  __ \ /\ \/\ \ /\  ___\ /\  ___\ /\__  _\/\  ___\    //
//  \ \ \/\_\\ \ \_\ \\ \  __\ \ \___  \\/_/\ \/\ \___  \   //
//   \ \___\_\\ \_____\\ \_____\\/\_____\  \ \_\ \/\_____\  //
//    \/___/_/ \/_____/ \/_____/ \/_____/   \/_/  \/_____/  //
//                                                          //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/quests.c

#include <math.h>

#include "game/quests.h"
#include "game/player.h"
#include "game/camera.h"
#include "game/aabb.h"
#include "game/game.h"

#include "platform/content.h"

#include "graphics/color.h"
#include "graphics/batch.h"

#include "audio/sound.h"

#include "math/mathf.h"
#include "math/vector.h"

#define QUESTS_NUM      (4)

#define QUEST_SIZE      (96)
#define QUEST_SIZE_HALF (QUEST_SIZE / 2.0)

enum
{
    QUEST_POSTCARD = 0,
    QUEST_PACKAGE,
    QUEST_BOTTLE,
    QUEST_LETTER,
};

static bool completed[QUESTS_NUM];

static float scales[QUESTS_NUM];

static float indicator_texture[4];
static float(textures[QUESTS_NUM])[4];

static vec2_t positions[QUESTS_NUM] = {
    [QUEST_POSTCARD] = {.x = 800, .y = -2560},
    [QUEST_PACKAGE]  = {.x = 256, .y = -8200},
    [QUEST_BOTTLE]   = {.x = 320, .y = -12880},
    [QUEST_LETTER]   = {.x = 640, .y = -25600},
};

static sound_t* quest_complete_sound;

static void quest_get_rect(size_t index, float rect[4]);

static void quest_render_indicator(size_t index, float dt, float total);

void quests_init(void)
{
    for (int i = 0; i < QUESTS_NUM; ++i)
    {
        scales[i]    = 1;
        completed[i] = false;
    }

    int(*texture)[4];
    int(*texture1)[4];
    int(*texture2)[4];
    int(*texture3)[4];
    int(*texture4)[4];

    content_find_textures("indicator", &texture);

    content_find_textures("lost1", &texture1);
    content_find_textures("lost1", &texture1);
    content_find_textures("lost2", &texture2);
    content_find_textures("lost3", &texture3);
    content_find_textures("lost4", &texture4);

    indicator_texture[0] = (*texture)[0];
    indicator_texture[1] = (*texture)[1];
    indicator_texture[2] = (*texture)[2];
    indicator_texture[3] = (*texture)[3];

    textures[QUEST_POSTCARD][0] = (*texture1)[0];
    textures[QUEST_POSTCARD][1] = (*texture1)[1];
    textures[QUEST_POSTCARD][2] = (*texture1)[2];
    textures[QUEST_POSTCARD][3] = (*texture1)[3];

    textures[QUEST_PACKAGE][0] = (*texture2)[0];
    textures[QUEST_PACKAGE][1] = (*texture2)[1];
    textures[QUEST_PACKAGE][2] = (*texture2)[2];
    textures[QUEST_PACKAGE][3] = (*texture2)[3];

    textures[QUEST_BOTTLE][0] = (*texture3)[0];
    textures[QUEST_BOTTLE][1] = (*texture3)[1];
    textures[QUEST_BOTTLE][2] = (*texture3)[2];
    textures[QUEST_BOTTLE][3] = (*texture3)[3];

    textures[QUEST_LETTER][0] = (*texture4)[0];
    textures[QUEST_LETTER][1] = (*texture4)[1];
    textures[QUEST_LETTER][2] = (*texture4)[2];
    textures[QUEST_LETTER][3] = (*texture4)[3];

    content_find_sounds("quest", &quest_complete_sound);
}

static void quest_get_rect(size_t index, float rect[4])
{
    rect[0] = positions[index].x;
    rect[1] = positions[index].y;
    rect[2] = QUEST_SIZE;
    rect[3] = QUEST_SIZE;
}

void quests_update(float dt, float total)
{
    static float quest_rect[4];
    static float player_rect[4];

    player_get_rect(player_rect);

    for (int i = 0; i < QUESTS_NUM; ++i)
    {
        if (completed[i])
        {
            scales[i] = mathf_max(0, scales[i] - 8 * dt);
            continue;
        }

        quest_get_rect(i, quest_rect);

        if (camera_within_bounds(quest_rect[1], quest_rect[3]))
        {
            if (aabb_check_overlap(quest_rect, player_rect))
            {
                completed[i] = true;

                camera_shake(0.1, 2);
                game_sleep(0.05);
                sound_play(quest_complete_sound, 1, false);
            }
        }
    }

    positions[QUEST_LETTER].x = 480 + sinf(total) * 320;
}

static void quest_render_indicator(size_t index, float dt, float total)
{
    float x, y;
    float zoom;

    vec2_t position = positions[index];

    camera_get_zoom(&zoom);
    camera_get_position(&x, &y);

    batch_set_tint(RGB_BLACK, 0.3);
    batch_set_fill(RGB_BLACK, 0);
    batch_set_rotation(
        (position.y > y ? 180 : 0),
        (float[2]){
            position.x + QUEST_SIZE_HALF + 8,
            y + (GAME_HEIGHT / zoom / 2.0 - 16) * (position.y > y ? 1 : -1) + 4,
        }
    );
    batch_draw_texture(
        indicator_texture,
        (float[4]){
            position.x + 8,
            y + (GAME_HEIGHT / zoom / 2.0 - 16) * (position.y > y ? 1 : -1) + 4,
            QUEST_SIZE,
            QUEST_SIZE,
        }
    );
    batch_set_rotation(0, VEC2_ZERO.xy);
    batch_set_tint(RGB_WHITE, 1);
    batch_set_rotation(
        (position.y > y ? 180 : 0),
        (float[2]){
            position.x + QUEST_SIZE_HALF,
            y + (GAME_HEIGHT / zoom / 2.0 - 16) * (position.y > y ? 1 : -1),
        }
    );
    batch_draw_texture(
        indicator_texture,
        (float[4]){
            position.x,
            y + (GAME_HEIGHT / zoom / 2.0 - 16) * (position.y > y ? 1 : -1),
            QUEST_SIZE,
            QUEST_SIZE,
        }
    );
    batch_set_rotation(0, VEC2_ZERO.xy);
    batch_draw_texture(
        textures[index],
        (float[4]){
            position.x + QUEST_SIZE_HALF / 2.0,
            (position.y > y ? y + (GAME_HEIGHT / zoom / 2.0 - 92) : y - (GAME_HEIGHT / zoom / 2.0 - 44)),
            QUEST_SIZE_HALF,
            QUEST_SIZE_HALF,
        }
    );
}

void quests_render(float dt, float total)
{
    float dst[4];
    float rect[4];

    for (int i = 0; i < 4; ++i)
    {
        quest_get_rect(i, rect);

        if (camera_within_bounds(rect[01], rect[3]))
        {
            float size_half_scaled = QUEST_SIZE_HALF * (1.0 - scales[i]);

            dst[0] = rect[0] + size_half_scaled;
            dst[1] = rect[1] + size_half_scaled + sinf(total * 4) * 16;
            dst[2] = rect[2] * scales[i];
            dst[3] = rect[3] * scales[i];

            batch_set_rotation(
                sinf((total)*2) * 15,
                (float[2]){
                    dst[0] + QUEST_SIZE_HALF - size_half_scaled,
                    dst[1] + QUEST_SIZE_HALF - size_half_scaled,
                }
            );
            batch_set_tint(RGB_WHITE, 1);
            batch_set_fill(RGB_WHITE, completed[i]);
            batch_draw_texture(textures[i], dst);
            batch_set_rotation(0, VEC2_ZERO.xy);
        }
        else if (!completed[i])
        {
            quest_render_indicator(i, dt, total);
        }
    }
}
