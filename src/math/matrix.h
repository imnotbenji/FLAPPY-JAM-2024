// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __    __   ______   ______  ______   __   __  __     //
//  /\ "-./  \ /\  __ \ /\__  _\/\  == \ /\ \ /\_\_\_\    //
//  \ \ \-./\ \\ \  __ \\/_/\ \/\ \  __< \ \ \\/_/\_\/_   //
//   \ \_\ \ \_\\ \_\ \_\  \ \_\ \ \_\ \_\\ \_\ /\_\/\_\  //
//    \/_/  \/_/ \/_/\/_/   \/_/  \/_/ /_/ \/_/ \/_/\/_/  //
//                                                        //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// math/matrix.h

#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#define MAT4_ZERO     \
    ((mat4_t){        \
        {0, 0, 0, 0}, \
        {0, 0, 0, 0}, \
        {0, 0, 0, 0}, \
        {0, 0, 0, 0}, \
    })

#define MAT4_IDENTITY \
    ((mat4_t){        \
        {1, 0, 0, 0}, \
        {0, 1, 0, 0}, \
        {0, 0, 1, 0}, \
        {0, 0, 0, 1}, \
    })

#ifdef __cplusplus
extern "C"
{
#endif

    typedef float mat4_t[4][4];

    void mat4_copy(mat4_t mat, mat4_t out);

    void mat4_mul(mat4_t a, mat4_t b, mat4_t out);

    void mat4_scale(float s, mat4_t out);
    void mat4_translate(float xy[2], mat4_t out);
    void mat4_orthographic(float left, float right, float bottom, float top, mat4_t out);

    void mat4_zero(mat4_t out);
    void mat4_identity(mat4_t out);

#ifdef __cplusplus
}
#endif

#endif  // MATH_MATRIX_H
