// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __   __ ______   ______   ______  ______   ______     //
//  /\ \ / //\  ___\ /\  ___\ /\__  _\/\  __ \ /\  == \    //
//  \ \ \'/ \ \  __\ \ \ \____\/_/\ \/\ \ \/\ \\ \  __<    //
//   \ \__|  \ \_____\\ \_____\  \ \_\ \ \_____\\ \_\ \_\  //
//    \/_/    \/_____/ \/_____/   \/_/  \/_____/ \/_/ /_/  //
//                                                         //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// math/vector.h

#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#define VEC2_ZERO  ((vec2_t){{0, 0}})
#define VEC2_ONE   ((vec2_t){{1, 1}})
#define VEC2_LEFT  ((vec2_t){{-1, 0}})
#define VEC2_RIGHT ((vec2_t){{1, 0}})
#define VEC2_UP    ((vec2_t){{0, -1}})
#define VEC2_DOWN  ((vec2_t){{0, 1}})

#ifdef __cplusplus
extern "C"
{
#endif

    typedef union
    {
        struct
        {
            float x, y;
        };

        float xy[2];
    } vec2_t;

    void vec2_copy(vec2_t v, vec2_t* out);

    void vec2_add(vec2_t a, vec2_t b, vec2_t* out);
    void vec2_sub(vec2_t a, vec2_t b, vec2_t* out);
    void vec2_mul(vec2_t a, vec2_t b, vec2_t* out);
    void vec2_div(vec2_t a, vec2_t b, vec2_t* out);

    void vec2_add_s(vec2_t v, float s, vec2_t* out);
    void vec2_sub_s(vec2_t v, float s, vec2_t* out);
    void vec2_mul_s(vec2_t v, float s, vec2_t* out);
    void vec2_div_s(vec2_t v, float s, vec2_t* out);

    void vec2_lerp(vec2_t a, vec2_t b, float t, vec2_t* out);

    float vec2_dot(vec2_t a, vec2_t b);
    void  vec2_normalize(vec2_t* out);

#ifdef __cplusplus
}
#endif

#endif  // MATH_VECTOR_H
