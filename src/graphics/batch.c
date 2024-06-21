// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   ______  ______   __  __     //
//  /\  == \ /\  __ \ /\__  _\/\  ___\ /\ \_\ \    //
//  \ \  __< \ \  __ \\/_/\ \/\ \ \____\ \  __ \   //
//   \ \_____\\ \_\ \_\  \ \_\ \ \_____\\ \_\ \_\  //
//    \/_____/ \/_/\/_/   \/_/  \/_____/ \/_/\/_/  //
//                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/batch.c

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#ifndef M_PI
#define M_PI (3.141592653585979323846)
#endif
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/batch.h"
#include "graphics/color.h"
#include "graphics/renderer.h"

#define VERTEX_PER_TRIANGLE (3)
#define VERTEX_PER_QUAD     (4)
#define VERTEX_PER_CUBE     (8)
#define INDEX_PER_TRIANGLE  (3)
#define INDEX_PER_QUAD      (INDEX_PER_TRIANGLE * 2)

typedef struct
{
    float x, y, z;
    float u, v;
    float r0, g0, b0, a0;
    float r1, g1, b1, a1;
} vertex_t;

typedef struct
{
    float x0, y0;
    float x1, y1;
    float x2, y2;
    float x3, y3;
} quad_desc_t;

static size_t quads_capacity = 0u;

static bool setup = false;
static bool began = false;

static uint32_t index_buffer;
static uint32_t vertex_buffer;
static uint32_t vertex_array;

static uint32_t* indices;
static vertex_t* vertices;
static vertex_t* vertices_write;

static size_t indices_len;
static size_t vertices_len;

static uint32_t default_texture_id;

static uint32_t tint;
static uint32_t fill;
static float    tint_alpha;
static float    fill_alpha;

static uint32_t flip;
static float    rotation;
static float    origin[2];

static CULL_MODE  cull;
static BLEND_MODE blend;

static uint32_t shader_id;

static uint32_t texture_id;
static int      texture_width;
static int      texture_height;

void batch_init(size_t capacity)
{
    assert(capacity > 0);

    vertex_array  = renderer_vertex_array_generate();
    vertex_buffer = renderer_vertex_buffer_generate_dynamic(capacity * VERTEX_PER_QUAD * sizeof(vertex_t));

    renderer_vertex_array_add_buffer(
        vertex_array, vertex_buffer, 4,
        (ATTRIBUTE_TYPE[]){
            ATTRIBUTE_VEC3,
            ATTRIBUTE_VEC2,
            ATTRIBUTE_VEC4,
            ATTRIBUTE_VEC4,
        }
    );

    index_buffer = renderer_index_buffer_generate_dynamic(capacity * INDEX_PER_QUAD);

    indices_len  = 0u;
    vertices_len = 0u;

    indices  = (uint32_t*)malloc(capacity * INDEX_PER_QUAD * sizeof(uint32_t));
    vertices = (vertex_t*)malloc(capacity * VERTEX_PER_QUAD * sizeof(vertex_t));

    uint8_t white_pixel[4] = {255, 255, 255, 255};
    default_texture_id     = renderer_texture_generate(white_pixel, 1, 1, TEXTURE_FORMAT_UBYTE);

    quads_capacity = capacity;
    setup          = true;
}

void batch_shutdown(void)
{
    renderer_index_buffer_delete(index_buffer);
    renderer_vertex_buffer_delete(vertex_buffer);
    renderer_vertex_array_delete(vertex_array);

    indices_len  = 0U;
    vertices_len = 0U;

    free(indices);
    free(vertices);

    quads_capacity = 0u;
    setup          = false;
}

static void batch_reset(void)
{
    indices_len    = 0U;
    vertices_len   = 0U;
    vertices_write = &vertices[0];
}

static void batch_flush(void)
{
    renderer_index_buffer_subdata(indices, indices_len * sizeof(uint32_t));
    renderer_vertex_buffer_subdata(vertices, vertices_len * sizeof(vertex_t));

    renderer_draw_elements(DRAW_TRIANGLES, indices_len);

    batch_reset();
}

void batch_begin(void)
{
    assert(setup);
    assert(!began);

    began = true;

    batch_reset();

    renderer_vertex_array_bind(vertex_array);
    renderer_vertex_buffer_bind(vertex_buffer);
}

void batch_end(void)
{
    assert(began);

    began = false;

    if (vertices_len > 0)
    {
        batch_flush();
    }
}

void batch_set_tint(uint32_t rgb, float alpha)
{
    tint       = rgb;
    tint_alpha = alpha;
}

void batch_set_fill(uint32_t rgb, float alpha)
{
    fill       = rgb;
    fill_alpha = alpha;
}

void batch_set_flip(uint32_t axis) { flip = axis; }

void batch_set_rotation(float degrees, float org[2])
{
    rotation = degrees * (M_PI / 180.0);
    memcpy(origin, org, 2 * sizeof(float));
}

void batch_set_cull(uint32_t face)
{
    if (cull != face && vertices_len > 0)
    {
        batch_flush();
    }

    cull = (CULL_MODE)face;

    if (face != CULL_NONE) renderer_enable_cull(face);
    else renderer_disable_cull();
}

void batch_set_blend(uint32_t factor)
{
    if (blend != factor && vertices_len > 0)
    {
        batch_flush();
    }

    blend = (BLEND_MODE)factor;

    if (factor != BLEND_NORMAL) renderer_enable_blend(factor);
    else renderer_disable_blend();
}

void batch_set_shader(uint32_t id)
{
    if (id != shader_id && vertices_len > 0)
    {
        batch_flush();
    }

    renderer_shader_bind(id);
}

void batch_set_texture(uint32_t id, int width, int height)
{
    if (id != texture_id && vertices_len > 0)
    {
        batch_flush();
    }

    texture_id     = id;
    texture_width  = width;
    texture_height = height;

    renderer_texture_bind(id, 0);
}

static void normalize_uvs(float x, float y, float width, float height, float normalized[4])
{
    normalized[0] = x / texture_width;
    normalized[1] = y / texture_height;
    normalized[2] = width / texture_width;
    normalized[3] = height / texture_height;
}

static vertex_t* push_quad(vertex_t* buffer, quad_desc_t src, quad_desc_t dst)
{
    float uvs[4];

    normalize_uvs(src.x0, src.y0, src.x1 - src.x0, src.y2 - src.y0, uvs);

    float bottom_left[2]  = {uvs[0], uvs[1]};
    float bottom_right[2] = {uvs[0] + uvs[2], uvs[1]};
    float top_left[2]     = {uvs[0], uvs[1] + uvs[3]};
    float top_right[2]    = {uvs[0] + uvs[2], uvs[1] + uvs[3]};

    float temp = 0;

    if ((flip & BATCH_FLIP_VERT) == BATCH_FLIP_VERT)
    {
        temp            = bottom_right[1];
        bottom_right[1] = top_left[1];
        top_left[1]     = temp;

        temp           = bottom_left[1];
        bottom_left[1] = top_right[1];
        top_right[1]   = temp;
    }

    if ((flip & BATCH_FLIP_HORZ) == BATCH_FLIP_HORZ)
    {
        temp            = bottom_right[0];
        bottom_right[0] = top_left[0];
        top_left[0]     = temp;

        temp           = bottom_left[0];
        bottom_left[0] = top_right[0];
        top_right[0]   = temp;
    }

    float dx0 = dst.x0 - origin[0];
    float dy0 = dst.y0 - origin[1];
    float dx1 = dst.x1 - origin[0];
    float dy1 = dst.y1 - origin[1];
    float dx2 = dst.x2 - origin[0];
    float dy2 = dst.y2 - origin[1];
    float dx3 = dst.x3 - origin[0];
    float dy3 = dst.y3 - origin[1];

    float sin = sinf(rotation);
    float cos = cosf(rotation);

    float x0 = dx0 * cos - dy0 * sin + origin[0];
    float y0 = dx0 * sin + dy0 * cos + origin[1];
    float x1 = dx1 * cos - dy1 * sin + origin[0];
    float y1 = dx1 * sin + dy1 * cos + origin[1];
    float x2 = dx2 * cos - dy2 * sin + origin[0];
    float y2 = dx2 * sin + dy2 * cos + origin[1];
    float x3 = dx3 * cos - dy3 * sin + origin[0];
    float y3 = dx3 * sin + dy3 * cos + origin[1];

    float rgb0[3];
    float rgb1[3];

    rgb_from_hex(tint, rgb0);
    rgb_from_hex(fill, rgb1);

    buffer->x  = x0;
    buffer->y  = y0;
    buffer->z  = 0;
    buffer->u  = bottom_left[0];
    buffer->v  = bottom_left[1];
    buffer->r0 = rgb0[0];
    buffer->g0 = rgb0[1];
    buffer->b0 = rgb0[2];
    buffer->a0 = tint_alpha;
    buffer->r1 = rgb1[0];
    buffer->g1 = rgb1[1];
    buffer->b1 = rgb1[2];
    buffer->a1 = fill_alpha;
    buffer++;

    buffer->x  = x1;
    buffer->y  = y1;
    buffer->z  = 0;
    buffer->u  = bottom_right[0];
    buffer->v  = bottom_right[1];
    buffer->r0 = rgb0[0];
    buffer->g0 = rgb0[1];
    buffer->b0 = rgb0[2];
    buffer->a0 = tint_alpha;
    buffer->r1 = rgb1[0];
    buffer->g1 = rgb1[1];
    buffer->b1 = rgb1[2];
    buffer->a1 = fill_alpha;
    buffer++;

    buffer->x  = x2;
    buffer->y  = y2;
    buffer->z  = 0;
    buffer->u  = top_right[0];
    buffer->v  = top_right[1];
    buffer->r0 = rgb0[0];
    buffer->g0 = rgb0[1];
    buffer->b0 = rgb0[2];
    buffer->a0 = tint_alpha;
    buffer->r1 = rgb1[0];
    buffer->g1 = rgb1[1];
    buffer->b1 = rgb1[2];
    buffer->a1 = fill_alpha;
    buffer++;

    buffer->x  = x3;
    buffer->y  = y3;
    buffer->z  = 0;
    buffer->u  = top_left[0];
    buffer->v  = top_left[1];
    buffer->r0 = rgb0[0];
    buffer->g0 = rgb0[1];
    buffer->b0 = rgb0[2];
    buffer->a0 = tint_alpha;
    buffer->r1 = rgb1[0];
    buffer->g1 = rgb1[1];
    buffer->b1 = rgb1[2];
    buffer->a1 = fill_alpha;
    buffer++;

    return buffer;
}

static void draw_quad(quad_desc_t src, quad_desc_t dst)
{
    assert(began);

    if (vertices_len * VERTEX_PER_QUAD >= quads_capacity)
    {
        batch_flush();
    }

    indices[indices_len + 0] = 0 + vertices_len;  //      2
    indices[indices_len + 1] = 2 + vertices_len;  //    / |
    indices[indices_len + 2] = 1 + vertices_len;  //  0---1
    indices[indices_len + 3] = 0 + vertices_len;  //  3---2
    indices[indices_len + 4] = 3 + vertices_len;  //  | /
    indices[indices_len + 5] = 2 + vertices_len;  //  0
    indices_len += INDEX_PER_QUAD;

    vertices_write = push_quad(vertices_write, src, dst);
    vertices_len += VERTEX_PER_QUAD;
}

void batch_draw_texture(float src[4], float dst[4])
{
    quad_desc_t src_rect = {
        .x0 = src[0],
        .y0 = src[1],
        .x1 = src[0] + src[2],
        .y1 = src[1],
        .x2 = src[0] + src[2],
        .y2 = src[1] + src[3],
        .x3 = src[0],
        .y3 = src[1] + src[3],
    };

    quad_desc_t dst_rect = {
        .x0 = dst[0],
        .y0 = dst[1],
        .x1 = dst[0] + dst[2],
        .y1 = dst[1],
        .x2 = dst[0] + dst[2],
        .y2 = dst[1] + dst[3],
        .x3 = dst[0],
        .y3 = dst[1] + dst[3],
    };

    draw_quad(src_rect, dst_rect);
}
