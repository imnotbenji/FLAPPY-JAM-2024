// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __   __   _____    ______   ______  ______  ______   ______   ______   ______  ______   //
//  /\  == \ /\  ___\ /\ "-.\ \ /\  __-. /\  ___\ /\  == \/\__  _\/\  __ \ /\  == \ /\  ___\ /\  ___\/\__  _\  //
//  \ \  __< \ \  __\ \ \ \-.  \\ \ \/\ \\ \  __\ \ \  __<\/_/\ \/\ \  __ \\ \  __< \ \ \__ \\ \  __\\/_/\ \/  //
//   \ \_\ \_\\ \_____\\ \_\\"\_\\ \____- \ \_____\\ \_\ \_\ \ \_\ \ \_\ \_\\ \_\ \_\\ \_____\\ \_____\ \ \_\  //
//    \/_/ /_/ \/_____/ \/_/ \/_/ \/____/  \/_____/ \/_/ /_/  \/_/  \/_/\/_/ \/_/ /_/ \/_____/ \/_____/  \/_/  //
//                                                                                                             //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/rendertarget.c

#include "graphics/rendertarget.h"
#include "graphics/renderer.h"

render_target_t render_target_generate(int width, int height, size_t num_color_buffers, uint32_t* color_types)
{
    uint32_t frame_buffer = renderer_frame_buffer_generate();

    render_target_t render_target = {
        .frame_buffer      = frame_buffer,
        .color_buffers_len = num_color_buffers,
        .width             = width,
        .height            = height,
    };

    for (int i = 0; i < num_color_buffers; ++i)
    {
        uint32_t color_type   = color_types[i];
        uint32_t color_buffer = renderer_frame_buffer_attachment_generate(width, height, color_type);

        render_target.color_types[i]   = color_type;
        render_target.color_buffers[i] = color_buffer;

        renderer_frame_buffer_attachment_bind(frame_buffer, color_buffer, ATTACHMENT_COLOR, i);
    }

    renderer_frame_buffer_unbind();

    return render_target;
}

void render_target_delete(render_target_t render_target)
{
    for (int i = 0; i < render_target.color_buffers_len; ++i)
    {
        renderer_frame_buffer_attachment_unbind(render_target.frame_buffer, ATTACHMENT_COLOR, i);
        renderer_texture_delete(render_target.color_buffers[i]);
    }

    renderer_frame_buffer_unbind();
    renderer_frame_buffer_delete(render_target.frame_buffer);
}

void render_target_resize(render_target_t* render_target, int width, int height)
{
    for (int i = 0; i < render_target->color_buffers_len; ++i)
    {
        uint32_t color_type        = render_target->color_types[i];
        uint32_t prev_color_buffer = render_target->color_buffers[i];
        uint32_t curr_color_buffer = renderer_frame_buffer_attachment_generate(width, height, color_type);

        render_target->color_buffers[i] = curr_color_buffer;

        renderer_frame_buffer_attachment_unbind(render_target->frame_buffer, ATTACHMENT_COLOR, i);
        renderer_texture_delete(prev_color_buffer);

        renderer_frame_buffer_attachment_bind(render_target->frame_buffer, curr_color_buffer, ATTACHMENT_COLOR, i);
    }

    renderer_frame_buffer_unbind();

    render_target->width  = width;
    render_target->height = height;
}
