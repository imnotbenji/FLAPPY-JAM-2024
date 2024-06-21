// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __   __ ______   ______   ______  ______   ______     //
//  /\ \ / //\  ___\ /\  ___\ /\__  _\/\  __ \ /\  == \    //
//  \ \ \'/ \ \  __\ \ \ \____\/_/\ \/\ \ \/\ \\ \  __<    //
//   \ \__|  \ \_____\\ \_____\  \ \_\ \ \_____\\ \_\ \_\  //
//    \/_/    \/_____/ \/_____/   \/_/  \/_____/ \/_/ /_/  //
//                                                         //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// math/vector.c

#include <math.h>

#include "math/vector.h"

void vec2_copy(vec2_t v, vec2_t* out)
{
    out->x = v.x;
    out->y = v.y;
}

void vec2_add(vec2_t a, vec2_t b, vec2_t* out)
{
    out->x = a.x + b.x;
    out->y = a.y + b.y;
}

void vec2_sub(vec2_t a, vec2_t b, vec2_t* out)
{
    out->x = a.x - b.x;
    out->y = a.y - b.y;
}

void vec2_mul(vec2_t a, vec2_t b, vec2_t* out)
{
    out->x = a.x * b.x;
    out->y = a.y * b.y;
}

void vec2_div(vec2_t a, vec2_t b, vec2_t* out)
{
    if (b.x != 0) out->x = a.x / b.x;
    else out->x = 0;

    if (b.y != 0) out->y = a.y / b.y;
    else out->y = 0;
}

void vec2_add_s(vec2_t v, float s, vec2_t* out)
{
    out->x = v.x + s;
    out->y = v.y + s;
}

void vec2_sub_s(vec2_t v, float s, vec2_t* out)
{
    out->x = v.x - s;
    out->y = v.y - s;
}

void vec2_mul_s(vec2_t v, float s, vec2_t* out)
{
    out->x = v.x * s;
    out->y = v.y * s;
}

void vec2_div_s(vec2_t v, float s, vec2_t* out)
{
    if (s != 0)
    {
        out->x = v.x / s;
        out->y = v.y / s;
    }
    else
    {
        *out = VEC2_ZERO;
    }
}

void vec2_lerp(vec2_t a, vec2_t b, float t, vec2_t* out)
{
    out->x = (1 - t) * a.x + t * b.x;
    out->y = (1 - t) * a.y + t * b.y;
}

float vec2_dot(vec2_t a, vec2_t b) { return a.x * b.x + a.y * b.y; }

void vec2_normalize(vec2_t* out)
{
    float dot       = vec2_dot(*out, *out);
    float magnitude = sqrtf(dot);

    if (magnitude > 1e-8) vec2_div_s(*out, magnitude, out);
    else *out = VEC2_ZERO;
}
