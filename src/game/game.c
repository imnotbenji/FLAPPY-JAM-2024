// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __    __   ______     //
//  /\  ___\ /\  __ \ /\ "-./  \ /\  ___\    //
//  \ \ \__ \\ \  __ \\ \ \-./\ \\ \  __\    //
//   \ \_____\\ \_\ \_\\ \_\ \ \_\\ \_____\  //
//    \/_____/ \/_/\/_/ \/_/  \/_/ \/_____/  //
//                                           //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/game.c

#include <stddef.h>
#include <string.h>

#include "game/game.h"
#include "game/camera.h"
#include "game/player.h"
#include "game/fruits.h"
#include "game/quests.h"
#include "game/particles.h"
#include "game/background.h"

#include "platform/platform.h"
#include "platform/content.h"

#include "graphics/quad.h"
#include "graphics/color.h"
#include "graphics/atlas.h"
#include "graphics/batch.h"
#include "graphics/shader.h"
#include "graphics/renderer.h"
#include "graphics/rendertarget.h"

#include "audio/audio.h"
#include "audio/sound.h"

#include "math/mathf.h"
#include "math/matrix.h"

static float sleep;

static bool paused;

static shader_t* sprite_shader;
static shader_t* backbuffer_shader;

static uint32_t atlas_id;
static int      atlas_width, atlas_height;

static render_target_t render_target;

void game_init(void)
{
    atlas_t* atlas = atlas_new((atlas_desc_t){
        .resolution = 4096,
        .capacity   = 128,
        .expand     = 4,
        .border     = 4,
    });

    uint8_t* pixels = NULL;

    content_load_shaders();
    content_load_sounds();
    content_load_textures_ex(atlas);

    atlas_pack(atlas);
    atlas_generate_texture(atlas, &pixels, &atlas_width, &atlas_height);

    atlas_id = renderer_texture_generate(pixels, atlas_width, atlas_height, TEXTURE_FORMAT_UBYTE);

    atlas_delete(atlas);

    content_find_shaders("sprite", &sprite_shader);
    content_find_shaders("backbuffer", &backbuffer_shader);

    render_target = render_target_generate(960, 1280, 1, (ATTACHMENT_TYPE[]){ATTACHMENT_UBYTE});

    batch_init(2048);
    audio_init();
    camera_init();
    player_init();
    fruits_init();
    quests_init();
    particles_init();
    background_init();

    sound_t* ambience;

    content_find_sounds("ambience", &ambience);
    sound_play(ambience, 1, true);

    batch_set_cull(CULL_BACK);
    batch_set_blend(BLEND_NON_PREMULTIPLIED);
}

void game_shutdown(void)
{
    render_target_delete(render_target);

    fruits_shutdown();
    particles_shutdown();

    content_unload_shaders();
    content_unload_textures();

    batch_shutdown();
    audio_shutdown();
}

void game_update(double dt, double _)
{
    if (!platform_is_window_active())
    {
        paused = true;
        return;
    }

    static double total = 0;

    total += dt;

    paused = false;

    audio_update(dt);
    camera_update(dt, total);
    quests_update(dt, total);

    if (sleep > 0)
    {
        sleep -= dt;
        return;
    }

    player_update(dt, total);
    fruits_update(dt, total);
    particles_update(dt, total);
}

void game_render(double dt, double _)
{
    static mat4_t matrix;

    static int(*texture)[4];

    static float x, y;
    static int   width, height;

    static double total = 0;

    if (platform_is_window_active())
    {
        total += dt;
    }

    platform_get_window_size(&width, &height);

    player_get_position(&x, &y);
    camera_set_position(GAME_WIDTH / 2.0, y);
    camera_set_zoom(mathf_min(mathf_max(mathf_lerp(0.6, 1.0, (y + 36000) / 21600.0), 0.6), 1.0));
    camera_get_position(&x, &y);
    camera_get_matrix(matrix);

    renderer_shader_bind(sprite_shader->id);
    shader_apply_uniform4x4f(sprite_shader, "matrix", matrix[0]);

    renderer_frame_buffer_bind(render_target.frame_buffer);
    renderer_viewport(0, 0, GAME_WIDTH, GAME_HEIGHT);
    renderer_clear_color();

    background_render(dt, total);

    batch_begin();
    batch_set_shader(sprite_shader->id);
    batch_set_texture(atlas_id, atlas_width, atlas_height);

    batch_set_tint(RGB_WHITE, 1);
    batch_set_fill(0, 0);

    if (y > -GAME_HEIGHT * 1.5)
    {
        content_find_textures("base", &texture);

        batch_draw_texture(
            (float[4]){
                (*texture)[0],
                (*texture)[1],
                (*texture)[2],
                (*texture)[3],
            },
            (float[4]){
                -8,
                -1160,
                GAME_WIDTH + 16,
                GAME_HEIGHT + 21,
            }
        );
    }

    if (y > -14000 && y < -10000)
    {
        content_find_textures("top", &texture);

        batch_draw_texture(
            (float[4]){
                (*texture)[0],
                (*texture)[1],
                (*texture)[2],
                (*texture)[3],
            },
            (float[4]){
                0,
                -12800,
                GAME_WIDTH,
                GAME_HEIGHT,
            }
        );
    }

    player_render(dt, total);
    fruits_render(dt, total);
    quests_render(dt, total);
    particles_render(dt, total);

    batch_end();

    batch_begin();
    batch_set_shader(sprite_shader->id);
    batch_set_texture(atlas_id, atlas_width, atlas_height);

    if (paused)
    {
        content_find_textures("paused", &texture);

        batch_set_tint(RGB_WHITE, 1);
        batch_draw_texture(
            (float[4]){
                (*texture)[0],
                (*texture)[1],
                (*texture)[2],
                (*texture)[3],
            },
            (float[4]){
                GAME_WIDTH / 2.0 - 564 / 2.0,
                y - 99 / 2.0,
                564,
                99,
            }
        );
    }

    batch_end();

    float ratio = GAME_WIDTH / (float)GAME_HEIGHT;

    int vw = width;
    int vh = (int)(width / ratio + 0.5);

    if (vh > height)
    {
        vh = height;
        vw = (int)(height * ratio + 0.5);
    }

    int vx = (width / 2) - (vw / 2);
    int vy = (height / 2) - (vh / 2);

    renderer_viewport(vx, vy, vw, vh);
    renderer_frame_buffer_unbind();
    renderer_shader_bind(backbuffer_shader->id);
    renderer_texture_bind(render_target.color_buffers[0], 0);

    shader_apply_uniformi(backbuffer_shader, "paused", paused);

    quad_draw();
}

void game_sleep(float ms) { sleep = ms; }
