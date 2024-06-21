// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __   __   _____    ______   ______  ______  ______   ______   ______   ______  ______   //
//  /\  == \ /\  ___\ /\ "-.\ \ /\  __-. /\  ___\ /\  == \/\__  _\/\  __ \ /\  == \ /\  ___\ /\  ___\/\__  _\  //
//  \ \  __< \ \  __\ \ \ \-.  \\ \ \/\ \\ \  __\ \ \  __<\/_/\ \/\ \  __ \\ \  __< \ \ \__ \\ \  __\\/_/\ \/  //
//   \ \_\ \_\\ \_____\\ \_\\"\_\\ \____- \ \_____\\ \_\ \_\ \ \_\ \ \_\ \_\\ \_\ \_\\ \_____\\ \_____\ \ \_\  //
//    \/_/ /_/ \/_____/ \/_/ \/_/ \/____/  \/_____/ \/_/ /_/  \/_/  \/_/\/_/ \/_/ /_/ \/_____/ \/_____/  \/_/  //
//                                                                                                             //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/rendertarget.h

#ifndef GRAPHICS_RENDER_TARGET_H
#define GRAPHICS_RENDER_TARGET_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_NUM_COLOR_BUFFERS (16)

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct render_target_t
    {
        uint32_t frame_buffer;

        size_t   color_buffers_len;
        uint32_t color_types[MAX_NUM_COLOR_BUFFERS];
        uint32_t color_buffers[MAX_NUM_COLOR_BUFFERS];

        int width;
        int height;
    } render_target_t;

    render_target_t render_target_generate(int width, int height, size_t num_color_buffers, uint32_t* color_types);
    void            render_target_delete(render_target_t render_target);

    void render_target_resize(render_target_t* render_target, int width, int height);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_RENDERTARGET_H
