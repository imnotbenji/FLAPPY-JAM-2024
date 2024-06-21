// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  __       ______   __  __   ______   ______     //
//  /\  == \/\ \     /\  __ \ /\ \_\ \ /\  ___\ /\  == \    //
//  \ \  _-/\ \ \____\ \  __ \\ \____ \\ \  __\ \ \  __<    //
//   \ \_\   \ \_____\\ \_\ \_\\/\_____\\ \_____\\ \_\ \_\  //
//    \/_/    \/_____/ \/_/\/_/ \/_____/ \/_____/ \/_/ /_/  //
//                                                          //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/player.c

#include <math.h>
#include <string.h>

#include "game/player.h"
#include "game/game.h"
#include "game/fruits.h"
#include "game/camera.h"
#include "game/particles.h"
#include "game/animation.h"

#include "platform/input.h"
#include "platform/content.h"
#include "platform/platform.h"

#include "audio/sound.h"

#include "graphics/color.h"
#include "graphics/batch.h"

#include "math/mathi.h"
#include "math/mathf.h"
#include "math/vector.h"

#define PLAYER_SIZE           (64)
#define PLAYER_SIZE_HALF      (PLAYER_SIZE / 2.0)
#define PLAYER_RENDER_SIZE    (96)

#define PLAYER_SPEED          (256)
#define PLAYER_JUMP           (16)

#define PLAYER_Y_VELOCITY_MAX (48)
#define PLAYER_VELOCITY_JUMP  (1)

#define PLAYER_STARS_MAX      (8)

enum
{
    PLAYER_GROUND = 0x0,
    PLAYER_FLYING,
    PLAYER_DIVING,
    PLAYER_GLIDER,
    PLAYER_NOMOVE,
};

static float player_gravity[] = {
    [PLAYER_GROUND] = 0,   //
    [PLAYER_FLYING] = 16,  //
    [PLAYER_DIVING] = 32,  //
    [PLAYER_GLIDER] = 8,   //
    [PLAYER_NOMOVE] = 8,   //
};

static float player_acceleration[] = {
    [PLAYER_GROUND] = 0.9,  //
    [PLAYER_FLYING] = 0.4,  //
    [PLAYER_DIVING] = 0.2,  //
    [PLAYER_GLIDER] = 0.2,  //
    [PLAYER_NOMOVE] = 0.1,  //
};

static float player_deceleration[] = {
    [PLAYER_GROUND] = 0.8,   //
    [PLAYER_FLYING] = 0.02,  //
    [PLAYER_DIVING] = 0.01,  //
    [PLAYER_GLIDER] = 0.01,  //
    [PLAYER_NOMOVE] = 0.0,   //
};

static animation_t  player_animation_idle;
static animation_t  player_animation_walk;
static animation_t  player_animation_jump;
static animation_t  player_animation_fall;
static animation_t  player_animation_glide;
static animation_t* player_animation;

static vec2_t player_position;
static vec2_t player_velocity;

static vec2_t player_scale;
static float  player_rotation;

static int player_state;
static int player_facing;
static int player_stars;

static float star_texture[4];

static sound_t* gust_sound;
static sound_t* flap_sound;
static sound_t* land_sound;
static sound_t* powerup_sound;
static sound_t* sparkle_sound;

static particle_system_t dust_particles;
static particle_system_t feathers_particles;

#include <string.h>

static void player_set_squish(float x, float y);
static void player_update_squish(float dt);

static void player_jump(void);
static void player_update_state(float dt);

static void player_set_animation(animation_t* animation);
static void player_update_animation(float dt);

void player_init(void)
{
    player_position = (vec2_t){{GAME_WIDTH / 2.0, 0}};
    player_velocity = VEC2_ZERO;

    player_scale    = VEC2_ONE;
    player_rotation = 0;
    player_facing   = 1;

    player_state = PLAYER_GROUND;

    player_stars = 0;

    player_animation_walk  = (animation_t){.frames_len = 2, .time_per_frame = 0.083, .loop = true};
    player_animation_idle  = (animation_t){.frames_len = 1};
    player_animation_jump  = (animation_t){.frames_len = 3, .time_per_frame = 0.083};
    player_animation_fall  = (animation_t){.frames_len = 3, .time_per_frame = 0.083};
    player_animation_glide = (animation_t){.frames_len = 1};

    player_animation = &player_animation_idle;

    int(*texture)[4];

    content_find_textures("idle", &texture);

    memcpy(
        player_animation_idle.frames[0],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("walk1", &texture);

    memcpy(
        player_animation_walk.frames[0],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("walk2", &texture);

    memcpy(
        player_animation_walk.frames[1],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("jump1", &texture);

    memcpy(
        player_animation_jump.frames[0],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("jump2", &texture);

    memcpy(
        player_animation_jump.frames[1],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("jump3", &texture);

    memcpy(
        player_animation_jump.frames[2],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("jump2", &texture);

    memcpy(
        player_animation_fall.frames[0],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("fall1", &texture);

    memcpy(
        player_animation_fall.frames[1],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("fall2", &texture);

    memcpy(
        player_animation_fall.frames[2],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("fall1", &texture);

    memcpy(
        player_animation_glide.frames[0],
        (int[4]){
            (*texture)[0],
            (*texture)[1],
            (*texture)[2],
            (*texture)[3],
        },
        4 * sizeof(int)
    );

    content_find_textures("star", &texture);

    star_texture[0] = (*texture)[0];
    star_texture[1] = (*texture)[1];
    star_texture[2] = (*texture)[2];
    star_texture[3] = (*texture)[3];

    feathers_particles = (particle_system_t){
        .width  = 64,
        .height = 64,
        .amount = 4,

        .spread    = 90,
        .direction = -45,

        .life_min = 0.2,
        .life_max = 1.6,

        .speed_min_start = 1,
        .speed_max_start = 4,

        .scale_min_start = 16,
        .scale_max_start = 32,

        .rotation_min_start = 0,
        .rotation_max_start = 360,
    };

    dust_particles = (particle_system_t){
        .width  = 64,
        .height = 64,
        .amount = 4,

        .spread    = 180,
        .direction = 45,

        .life_min = 0.2,
        .life_max = 1.6,

        .speed_min_start = -0.1,
        .speed_max_start = 0.1,

        .scale_min_start = 16,
        .scale_max_start = 32,

        .rotation_min_start = 0,
        .rotation_max_start = 360,
    };

    content_find_textures("feather", &texture);

    feathers_particles.src[0] = (*texture)[0];
    feathers_particles.src[1] = (*texture)[1];
    feathers_particles.src[2] = (*texture)[2];
    feathers_particles.src[3] = (*texture)[3];

    content_find_textures("dust", &texture);

    dust_particles.src[0] = (*texture)[0];
    dust_particles.src[1] = (*texture)[1];
    dust_particles.src[2] = (*texture)[2];
    dust_particles.src[3] = (*texture)[3];

    content_find_sounds("gust", &gust_sound);
    content_find_sounds("flap", &flap_sound);
    content_find_sounds("land", &land_sound);
    content_find_sounds("powerup", &powerup_sound);
    content_find_sounds("sparkle", &sparkle_sound);
}

static void player_set_squish(float x, float y)
{
    player_scale.x = x;
    player_scale.y = y;
}

static void player_update_squish(float dt) { vec2_lerp(player_scale, VEC2_ONE, dt * 6, &player_scale); }

static void player_jump(void)
{
    bool should_jump_big = player_state == PLAYER_GROUND || player_stars == PLAYER_STARS_MAX;

    if (should_jump_big)
    {
        player_velocity.y = -PLAYER_JUMP * 4;
        player_stars /= 2;

        camera_shake(0.8, 3);
        sound_play(gust_sound, 1, false);
    }
    else
    {
        player_velocity.y = -PLAYER_JUMP;
        player_stars      = mathf_max(0, player_stars - 1);
    }

    if (player_state == PLAYER_GROUND)
    {
        fruits_spawn();

        dust_particles.x = player_position.x + PLAYER_SIZE_HALF;
        dust_particles.y = player_position.y + PLAYER_SIZE_HALF;

        dust_particles.amount = mathi_random(2, 6);

        particle_system_emit(dust_particles);
    }

    player_animation_jump.time_elapsed = 0;

    player_set_squish(0.5, 1.5);

    feathers_particles.x = player_position.x + PLAYER_SIZE_HALF;
    feathers_particles.y = player_position.y + PLAYER_SIZE_HALF;

    feathers_particles.amount = mathi_random(3, 5);

    particle_system_emit(feathers_particles);
    sound_play(flap_sound, 2, false);
}

static void player_update_state(float dt)
{
    switch (player_state)
    {
        case PLAYER_GROUND:
            player_position.y = 0;
            player_velocity.y = 0;
            player_rotation   = 0;
            player_stars      = 0;
            break;

        case PLAYER_DIVING:
            player_set_squish(0.8, 1.2);

            feathers_particles.x = player_position.x + PLAYER_SIZE_HALF;
            feathers_particles.y = player_position.y + PLAYER_SIZE_HALF;

            feathers_particles.amount = 1;

            particle_system_emit(feathers_particles);
            break;

        case PLAYER_GLIDER: player_velocity.y = mathf_min(player_velocity.y, PLAYER_VELOCITY_JUMP); break;
    }
}

static void mouse_get_position_ndc(float* x, float* y)
{
    float nx, ny;
    int   mx, my;
    int   width, height;

    input_mouse_get_position(&mx, &my);
    platform_get_window_size(&width, &height);

    nx = (2 * mx) / (float)width - 1;
    ny = 1 - (2 * my) / (float)height;

    ny *= -1;

    nx += 1;
    ny += 1;

    nx *= 0.5;
    ny *= 0.5;

    *x = nx;
    *y = ny;
}

void player_update(float dt, float total)
{
    vec2_t mouse_position;
    vec2_t camera_position;

    camera_get_position(&camera_position.x, &camera_position.y);
    mouse_get_position_ndc(&mouse_position.x, &mouse_position.y);

    bool mouse_left  = mouse_position.x < (player_position.x - PLAYER_SIZE) / GAME_WIDTH;
    bool mouse_right = mouse_position.x > (player_position.x + PLAYER_SIZE) / GAME_WIDTH;
    bool mouse_above = mouse_position.y < ((camera_position.x - player_position.y - PLAYER_SIZE) / GAME_HEIGHT + 1) * 0.5;
    bool mouse_below = mouse_position.y > ((camera_position.y - player_position.y + PLAYER_SIZE * 4) / GAME_HEIGHT + 1) * 0.5;

    bool left_down  = input_key_down(KEY_A) || input_key_down(KEY_LEFT) || (input_mouse_down(MOUSE_LEFT) && mouse_left);
    bool right_down = input_key_down(KEY_D) || input_key_down(KEY_RIGHT) || (input_mouse_down(MOUSE_LEFT) && mouse_right);
    bool move_down  = left_down || right_down;

    bool jump_down    = input_key_down(KEY_W) || input_key_down(KEY_UP) || (input_mouse_down(MOUSE_LEFT) && mouse_above);
    bool jump_pressed = input_key_pressed(KEY_W) || input_key_pressed(KEY_UP) || input_mouse_click(MOUSE_LEFT);

    bool dive_down = input_key_down(KEY_S) || input_key_down(KEY_DOWN) || (input_mouse_down(MOUSE_LEFT) && mouse_below);

    bool can_jump  = player_state == PLAYER_GROUND || (player_velocity.y > PLAYER_VELOCITY_JUMP && player_stars > 0);
    bool can_glide = player_velocity.y > PLAYER_VELOCITY_JUMP && !dive_down;
    bool can_dive  = player_state == PLAYER_FLYING && player_velocity.y > 0;

    float gravity = player_gravity[player_state];
    float easing  = move_down ? player_acceleration[player_state] : player_deceleration[player_state];

    int x = 0;

    x -= left_down;
    x += right_down;

    if (move_down && x != 0)
    {
        player_facing = mathf_sign(x);
    }

    if (jump_down && can_glide)
    {
        player_state = PLAYER_GLIDER;
    }
    else if (player_state == PLAYER_GLIDER)
    {
        player_state = PLAYER_FLYING;
    }

    if (dive_down && can_dive)
    {
        player_state = PLAYER_DIVING;
    }
    else if (player_state == PLAYER_DIVING)
    {
        player_state = PLAYER_FLYING;
    }

    if (jump_pressed && can_jump)
    {
        player_jump();
        player_state = PLAYER_FLYING;
    }

    player_update_state(dt);

    player_velocity.y += gravity * dt;
    player_velocity.y = mathf_min(player_velocity.y, PLAYER_Y_VELOCITY_MAX * (dive_down ? 2 : 1));

    float target_velocity = x * PLAYER_SPEED * (player_state == PLAYER_GLIDER ? 1.5 : 1) * dt;
    float target_rotation = move_down && player_state == PLAYER_FLYING ? x * -mathf_sign(player_velocity.y) * mathf_min(1, mathf_abs(player_velocity.y) / 8) * 15 : 0;

    target_rotation = player_position.x <= 0 || player_position.x >= GAME_WIDTH - PLAYER_SIZE_HALF ? 0 : target_rotation;

    player_velocity.x = mathf_lerp(player_velocity.x, target_velocity, easing);
    player_rotation   = mathf_lerp(player_rotation, target_rotation, 0.1);

    vec2_add(player_velocity, player_position, &player_position);

    player_position.x = mathf_clamp(player_position.x, 0, GAME_WIDTH - PLAYER_SIZE_HALF);

    if (player_position.y >= 0)
    {
        if (player_state != PLAYER_GROUND)
        {
            float time   = mathf_min(player_velocity.y / 100, 0.3);
            float amount = player_velocity.y / 10;

            player_set_squish(1.5, 0.75);

            dust_particles.x = player_position.x + PLAYER_SIZE_HALF;
            dust_particles.y = player_position.y + PLAYER_SIZE_HALF;

            dust_particles.amount = mathi_random(2, 6);

            particle_system_emit(dust_particles);
            camera_shake(time, amount);
            sound_play(land_sound, 1, false);
        }

        player_state = PLAYER_GROUND;
    }

    player_update_squish(dt);
    player_update_animation(dt);
}

static void player_set_animation(animation_t* animation)
{
    if (player_animation != animation)
    {
        animation->time_elapsed = 0;
        player_animation        = animation;
    }
}

static void player_update_animation(float dt)
{
    switch (player_state)
    {
        case PLAYER_GROUND:
            if (mathf_abs(player_velocity.x) < 0.3)
            {
                player_set_animation(&player_animation_idle);
            }
            else
            {
                player_set_animation(&player_animation_walk);
            }
            break;

        case PLAYER_FLYING:
            if (player_velocity.y < 0)
            {
                player_set_animation(&player_animation_jump);
            }
            else
            {
                player_set_animation(&player_animation_fall);
            }
            break;

        case PLAYER_GLIDER: player_set_animation(&player_animation_glide); break;

        case PLAYER_NOMOVE: break;
    }

    animation_step(player_animation, dt);
}

static void player_render_stars(float dt, float total)
{
    batch_set_fill(RGB_WHITE, 0);

    for (int i = 0; i < player_stars; ++i)
    {
        float radius = 72;
        float theta  = (2 * PI / player_stars) * i;

        float x = radius * cosf(theta + total * (1 + player_stars * 0.2));
        float y = radius * sinf(theta + total * (1 + player_stars * 0.2));

        float dst[4] = {
            x + player_position.x + 8,
            y + player_position.y,
            16,
            16,
        };

        uint32_t color = player_stars >= PLAYER_STARS_MAX ? 0xffffaa : RGB_WHITE;

        for (int j = 0; j < 3; ++j)
        {
            float x = radius * cosf(theta + total * (1 + player_stars * 0.2) - 0.1 * j);
            float y = radius * sinf(theta + total * (1 + player_stars * 0.2) - 0.1 * j);

            float dst[4] = {
                x + player_position.x + 8,
                y + player_position.y,
                16,
                16,
            };

            batch_set_tint(color, 1 - j / (float)3);
            batch_draw_texture(star_texture, dst);
        }

        if (player_stars >= PLAYER_STARS_MAX)
        {
            float t = fmod(total / 0.5, 1.0);

            float width  = 48 * t;
            float height = 48 * t;

            float dst[4] = {
                x + player_position.x + 8 - width / 2,
                y + player_position.y - height / 2,
                16 + width,
                16 + height,
            };

            batch_set_tint(color, 1 - t);
            batch_draw_texture(star_texture, dst);
        }

        batch_set_tint(color, 1);
        batch_draw_texture(star_texture, dst);
    }
}

void player_render(float dt, float total)
{
    vec2_t center;

    vec2_copy(player_position, &center);
    vec2_add_s(center, PLAYER_SIZE_HALF, &center);

    batch_set_tint(RGB_WHITE, 1);
    batch_set_fill(RGB_BLACK, 0);
    batch_set_flip(player_facing < 0 ? BATCH_FLIP_HORZ : BATCH_FLIP_NONE);
    batch_set_rotation(player_rotation, center.xy);

    uint8_t frame = animation_get_frame_index(*player_animation);

    batch_draw_texture(
        (float[4]){
            player_animation->frames[frame][0],
            player_animation->frames[frame][1],
            player_animation->frames[frame][2],
            player_animation->frames[frame][3],
        },
        (float[4]){
            player_position.x + PLAYER_RENDER_SIZE * 0.5 - PLAYER_RENDER_SIZE * 0.5 * player_scale.x - PLAYER_SIZE_HALF,
            player_position.y + PLAYER_RENDER_SIZE - PLAYER_RENDER_SIZE * player_scale.y - PLAYER_SIZE_HALF * 2,
            PLAYER_RENDER_SIZE * player_scale.x,
            PLAYER_RENDER_SIZE * player_scale.y,
        }
    );

    batch_set_rotation(0, VEC2_ZERO.xy);
    batch_set_flip(BATCH_FLIP_NONE);

    player_render_stars(dt, total);
}

void player_get_position(float* x, float* y)
{
    *x = player_position.x;
    *y = player_position.y;
}

void player_get_rect(float rect[4])
{
    rect[0] = player_position.x;
    rect[1] = player_position.y;
    rect[2] = PLAYER_SIZE_HALF;
    rect[3] = PLAYER_SIZE_HALF;
}

void player_add_star(void)
{
    static int cycle;
    cycle = (cycle + 1) % 6;

    sound_t* note;

    switch (cycle)
    {
        case 0: content_find_sounds("note1", &note); break;
        case 1: content_find_sounds("note2", &note); break;
        case 2: content_find_sounds("note3", &note); break;
        case 3: content_find_sounds("note4", &note); break;
        case 4: content_find_sounds("note5", &note); break;
        case 5: content_find_sounds("note6", &note); break;
    }

    sound_play(sparkle_sound, 1, false);

    if (player_stars + 1 == PLAYER_STARS_MAX)
    {
        sound_play(powerup_sound, 1, false);
    }
    else
    {
        sound_play(note, 1, false);
    }

    player_stars = mathf_min(player_stars + 1, PLAYER_STARS_MAX);
}
