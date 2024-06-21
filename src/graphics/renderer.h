// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __   __   _____    ______   ______   ______   ______     //
//  /\  == \ /\  ___\ /\ "-.\ \ /\  __-. /\  ___\ /\  == \ /\  ___\ /\  == \    //
//  \ \  __< \ \  __\ \ \ \-.  \\ \ \/\ \\ \  __\ \ \  __< \ \  __\ \ \  __<    //
//   \ \_\ \_\\ \_____\\ \_\\"\_\\ \____- \ \_____\\ \_\ \_\\ \_____\\ \_\ \_\  //
//    \/_/ /_/ \/_____/ \/_/ \/_/ \/____/  \/_____/ \/_/ /_/ \/_____/ \/_/ /_/  //
//                                                                              //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/renderer.h

#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum ATTRIBUTE_TYPE
    {
        ATTRIBUTE_BYTE,
        ATTRIBUTE_BOOL,
        ATTRIBUTE_INT,
        ATTRIBUTE_FLOAT,
        ATTRIBUTE_VEC2,
        ATTRIBUTE_VEC3,
        ATTRIBUTE_VEC4,
        ATTRIBUTE_IVEC2,
        ATTRIBUTE_IVEC3,
        ATTRIBUTE_IVEC4,
        ATTRIBUTE_UVEC2,
        ATTRIBUTE_UVEC3,
        ATTRIBUTE_UVEC4,
        ATTRIBUTE_MAT4,
    } ATTRIBUTE_TYPE;

    typedef enum DRAW_MODE
    {
        DRAW_LINES,
        DRAW_TRIANGLES,
        DRAW_TRIANGLE_STRIP,
        DRAW_QUADS,
    } DRAW_MODE;

    typedef enum CULL_MODE
    {
        CULL_NONE,
        CULL_BACK,
        CULL_FRONT,
    } CULL_MODE;

    typedef enum BLEND_MODE
    {
        BLEND_NORMAL,
        BLEND_NON_PREMULTIPLIED,
        BLEND_SUBTRACT,
        BLEND_ADDITIVE,
    } BLEND_MODE;

    typedef enum TEXTURE_FORMAT
    {
        TEXTURE_FORMAT_UBYTE,
        TEXTURE_FORMAT_FLOAT
    } TEXTURE_FORMAT;

    typedef enum TEXTURE_FILTER
    {
        TEXTURE_FILTER_NEAREST,
        TEXTURE_FILTER_LINEAR,
    } TEXTURE_FILTER;

    typedef enum TEXTURE_WRAP
    {
        TEXTURE_WRAP_CLAMP,
        TEXTURE_WRAP_REPEAT,
        TEXTURE_WRAP_MIRROR,
        TEXTURE_WRAP_BORDER,
    } TEXTURE_WRAP;

    typedef enum ATTACHMENT_TYPE
    {
        ATTACHMENT_NONE,
        ATTACHMENT_FLOAT,
        ATTACHMENT_UBYTE,
        ATTACHMENT_COLOR,
    } ATTACHMENT_TYPE;

    void renderer_bind(void* (*fn)(const char*));

    void renderer_viewport(int x, int y, int width, int height);

    void renderer_clear_color(void);
    void renderer_clear_color_set(float r, float g, float b);

    void renderer_enable_cull(CULL_MODE cull);
    void renderer_enable_blend(BLEND_MODE blend);

    void renderer_disable_cull(void);
    void renderer_disable_blend(void);

    void renderer_draw_arrays(DRAW_MODE mode, size_t vertices_len);
    void renderer_draw_elements(DRAW_MODE mode, size_t indices_len);

    uint32_t renderer_index_buffer_generate_static(void* data, size_t size);
    uint32_t renderer_index_buffer_generate_dynamic(size_t size);
    uint32_t renderer_vertex_buffer_generate_static(void* data, size_t size);
    uint32_t renderer_vertex_buffer_generate_dynamic(size_t size);
    uint32_t renderer_vertex_array_generate(void);
    uint32_t renderer_texture_generate(const void* pixels, int width, int height, TEXTURE_FORMAT format);
    uint32_t renderer_shader_generate(const char* vertex_shader_source, const char* fragment_shader_source);
    uint32_t renderer_frame_buffer_generate(void);
    uint32_t renderer_frame_buffer_attachment_generate(int width, int height, ATTACHMENT_TYPE attachment);

    void renderer_index_buffer_bind(uint32_t id);
    void renderer_index_buffer_unbind(void);
    void renderer_index_buffer_delete(uint32_t id);

    void renderer_vertex_buffer_bind(uint32_t id);
    void renderer_vertex_buffer_unbind(void);
    void renderer_vertex_buffer_delete(uint32_t id);

    void renderer_vertex_array_bind(uint32_t id);
    void renderer_vertex_array_unbind(void);
    void renderer_vertex_array_delete(uint32_t id);

    void renderer_texture_bind(uint32_t id, uint32_t slot);
    void renderer_texture_unbind(void);
    void renderer_texture_delete(uint32_t id);

    void renderer_shader_bind(uint32_t id);
    void renderer_shader_unbind(void);
    void renderer_shader_delete(uint32_t id);

    void renderer_frame_buffer_bind(uint32_t id);
    void renderer_frame_buffer_unbind(void);
    void renderer_frame_buffer_delete(uint32_t id);

    void renderer_frame_buffer_attachment_bind(uint32_t id, uint32_t texture, ATTACHMENT_TYPE attachment, int slot);
    void renderer_frame_buffer_attachment_unbind(uint32_t id, ATTACHMENT_TYPE attachment, int slot);

    void renderer_vertex_array_add_buffer(uint32_t id, uint32_t vertex_buffer_id, size_t layout_len, ATTRIBUTE_TYPE* layout);
    void renderer_vertex_buffer_subdata(void* data, size_t size);
    void renderer_index_buffer_subdata(void* data, size_t size);

    void renderer_texture_set_wrap(TEXTURE_WRAP wrap);
    void renderer_texture_set_filter(TEXTURE_FILTER filter);

    int  renderer_shader_get_uniform_location(uint32_t id, const char* name);
    void renderer_shader_set_uniformi(int location, int value);
    void renderer_shader_set_uniformf(int location, float value);
    void renderer_shader_set_uniform2f(int location, float* value);
    void renderer_shader_set_uniform3f(int location, float* value);
    void renderer_shader_set_uniform4f(int location, float* value);
    void renderer_shader_set_uniformfv(int location, size_t len, float* value);
    void renderer_shader_set_uniform4x4f(int location, float* value);

    void renderer_frame_buffer_copy(uint32_t from, uint32_t to, int src[4], int dst[4], ATTACHMENT_TYPE attachments);
    void renderer_frame_buffer_draw(size_t buffers_len);
    void renderer_frame_buffer_no_draw(void);
    void renderer_frame_buffer_attachment_depth_compare(void);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_RENDERER_H
