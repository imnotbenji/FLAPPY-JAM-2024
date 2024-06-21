// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   ______  ______   __  __     //
//  /\  == \ /\  __ \ /\__  _\/\  ___\ /\ \_\ \    //
//  \ \  __< \ \  __ \\/_/\ \/\ \ \____\ \  __ \   //
//   \ \_____\\ \_\ \_\  \ \_\ \ \_____\\ \_\ \_\  //
//    \/_____/ \/_/\/_/   \/_/  \/_____/ \/_/\/_/  //
//                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/batch.h

#ifndef GRAPHICS_BATCH_H
#define GRAPHICS_BATCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    enum
    {
        BATCH_FLIP_NONE = 0x0,
        BATCH_FLIP_HORZ = 1 << 0,
        BATCH_FLIP_VERT = 1 << 1,
        BATCH_FLIP_BOTH = BATCH_FLIP_HORZ | BATCH_FLIP_VERT,
    };

    void batch_init(size_t capacity);
    void batch_shutdown(void);

    void batch_begin(void);
    void batch_end(void);

    void batch_set_tint(uint32_t rgb, float alpha);
    void batch_set_fill(uint32_t rgb, float alpha);
    void batch_set_flip(uint32_t axis);
    void batch_set_rotation(float degrees, float origin[2]);
    void batch_set_cull(uint32_t face);
    void batch_set_blend(uint32_t factor);
    void batch_set_shader(uint32_t id);
    void batch_set_texture(uint32_t id, int width, int height);

    void batch_draw_texture(float src[4], float dst[4]);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_BATCH_H
