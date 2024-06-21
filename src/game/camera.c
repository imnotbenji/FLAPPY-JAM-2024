// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __    __   ______   ______   ______     //
//  /\  ___\ /\  __ \ /\ "-./  \ /\  ___\ /\  == \ /\  __ \    //
//  \ \ \____\ \  __ \\ \ \-./\ \\ \  __\ \ \  __< \ \  __ \   //
//   \ \_____\\ \_\ \_\\ \_\ \ \_\\ \_____\\ \_\ \_\\ \_\ \_\  //
//    \/_____/ \/_/\/_/ \/_/  \/_/ \/_____/ \/_/ /_/ \/_/\/_/  //
//                                                             //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// game/camera.c

#include "game/camera.h"
#include "game/game.h"

#include "math/mathi.h"
#include "math/mathf.h"
#include "math/vector.h"
#include "math/matrix.h"

#define CAMERA_DISTANCE_FROM_TARGET (128)

#define CAMERA_EASING               (0.05)
#define CAMERA_BOTTOM_Y             (512)

static vec2_t camera_position;
static vec2_t camera_target;
static float  camera_zoom;

static vec2_t shake_offset;
static float  shake_time;
static float  shake_amount;

void camera_init(void)
{
    camera_position = (vec2_t){{GAME_WIDTH / 2.0, -512}};
    camera_target   = VEC2_ZERO;
    camera_zoom     = 1;

    shake_offset = VEC2_ZERO;
    shake_time   = 0;
    shake_amount = 0;
}

void camera_update(float dt, float total)
{
    static vec2_t new_position;
    static vec2_t difference;

    vec2_lerp(camera_position, camera_target, CAMERA_EASING, &new_position);
    vec2_sub(new_position, camera_target, &difference);

    float magnitude = vec2_dot(difference, difference);

    if (magnitude > CAMERA_DISTANCE_FROM_TARGET * CAMERA_DISTANCE_FROM_TARGET)
    {
        vec2_normalize(&difference);
        vec2_mul_s(difference, CAMERA_DISTANCE_FROM_TARGET, &difference);
        vec2_add(camera_target, difference, &new_position);
    }

    vec2_copy(new_position, &camera_position);

    camera_position.y = mathf_min(camera_position.y, -CAMERA_BOTTOM_Y);

    vec2_copy(VEC2_ZERO, &shake_offset);

    if (shake_time > 0)
    {
        shake_time -= dt;

        shake_offset.x = mathi_random(-1, 1) * shake_amount;
        shake_offset.y = mathi_random(-1, 1) * shake_amount;
    }
}

void camera_set_zoom(float zoom) { camera_zoom = zoom; }

void camera_set_position(float x, float y) { camera_target = (vec2_t){{x, y}}; }

void camera_get_zoom(float* zoom) { *zoom = camera_zoom; }

void camera_get_position(float* x, float* y)
{
    *x = camera_position.x;
    *y = camera_position.y;
}

void camera_get_matrix(float out[4][4])
{
    static mat4_t view;
    static mat4_t projection;
    static mat4_t scale;
    static mat4_t translation;

    static vec2_t screen;
    static vec2_t center;
    static vec2_t offset;

    vec2_copy((vec2_t){{GAME_WIDTH, GAME_HEIGHT}}, &screen);
    vec2_div_s(screen, 2, &center);
    vec2_mul_s(camera_position, -1, &offset);

    vec2_sub(offset, shake_offset, &offset);

    mat4_identity(view);
    mat4_identity(projection);
    mat4_identity(scale);
    mat4_identity(translation);

    mat4_scale(camera_zoom, scale);
    mat4_translate(center.xy, view);
    mat4_translate(offset.xy, translation);
    mat4_orthographic(0, GAME_WIDTH, GAME_HEIGHT, 0, projection);

    mat4_mul(view, scale, view);
    mat4_mul(view, translation, view);
    mat4_mul(projection, view, out);
}

void camera_shake(float time, float amount)
{
    shake_time   = time;
    shake_amount = amount;
}

bool camera_within_bounds(float y, float height)
{
    float top    = camera_position.y - (GAME_HEIGHT / 2.0) / camera_zoom;
    float bottom = camera_position.y + (GAME_HEIGHT / 2.0) / camera_zoom;

    return y + height > top && y < bottom;
}
