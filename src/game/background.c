// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   ______   __  __   ______   ______   ______   __  __   __   __   _____     //
//  /\  == \ /\  __ \ /\  ___\ /\ \/ /  /\  ___\ /\  == \ /\  __ \ /\ \/\ \ /\ "-.\ \ /\  __-.   //
//  \ \  __< \ \  __ \\ \ \____\ \  _"-.\ \ \__ \\ \  __< \ \ \/\ \\ \ \_\ \\ \ \-.  \\ \ \/\ \  //
//   \ \_____\\ \_\ \_\\ \_____\\ \_\ \_\\ \_____\\ \_\ \_\\ \_____\\ \_____\\ \_\\"\_\\ \____-  //
//    \/_____/ \/_/\/_/ \/_____/ \/_/\/_/ \/_____/ \/_/ /_/ \/_____/ \/_____/ \/_/ \/_/ \/____/  //
//                                                                                               //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/background.c

#include "game/background.h"
#include "game/camera.h"
#include "game/game.h"

#include "platform/content.h"
#include "platform/platform.h"

#include "graphics/quad.h"
#include "graphics/color.h"
#include "graphics/batch.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/renderer.h"

#include "math/mathf.h"
#include "math/matrix.h"

static uint32_t bg_id;
static int      bg_width, bg_height;

static shader_t* sprite_shader;
static shader_t* scroll_shader;
static shader_t* stars_shader;

void background_init(void)
{
    texture_t* bg = texture_new_from_file("assets/textures/mid.png");

    bg_id     = renderer_texture_generate(bg->pixels, bg->width, bg->height, TEXTURE_FORMAT_UBYTE);
    bg_width  = bg->width;
    bg_height = bg->height;

    texture_delete_stb(bg);

    content_find_shaders("sprite", &sprite_shader);
    content_find_shaders("scroll", &scroll_shader);
    content_find_shaders("stars", &stars_shader);
}

void background_render(float dt, float total)
{
    mat4_t matrix;

    float x, y;
    int   width, height;

    platform_get_window_size(&width, &height);

    camera_get_position(&x, &y);
    camera_get_matrix(matrix);

    if (y < -11520)
    {
        renderer_shader_bind(stars_shader->id);

        shader_apply_uniformf(stars_shader, "scroll", y);
        shader_apply_uniformf(stars_shader, "time", total);
        shader_apply_uniform2f(stars_shader, "resolution", (float[2]){GAME_WIDTH, GAME_HEIGHT});

        quad_draw();
    }

    if (y > -12800)
    {
        renderer_shader_bind(scroll_shader->id);

        shader_apply_uniformf(scroll_shader, "scroll", mathf_max(y - GAME_HEIGHT / 2.0, -12800 + GAME_HEIGHT));
        shader_apply_uniformf(scroll_shader, "height", GAME_HEIGHT);
        shader_apply_uniform4x4f(scroll_shader, "matrix", matrix[0]);

        batch_set_texture(bg_id, bg_width, bg_height);
        batch_set_shader(scroll_shader->id);
        batch_begin();
        batch_set_tint(RGB_WHITE, 1);
        batch_draw_texture(
            (float[4]){
                0,
                0,
                GAME_WIDTH,
                GAME_HEIGHT,
            },
            (float[4]){
                0,
                mathf_max(y - GAME_HEIGHT / 2.0, -12800 + GAME_HEIGHT),
                GAME_WIDTH,
                GAME_HEIGHT,
            }
        );
        batch_end();
    }
}
