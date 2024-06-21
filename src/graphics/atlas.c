// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______  __       ______   ______     //
//  /\  __ \ /\__  _\/\ \     /\  __ \ /\  ___\    //
//  \ \  __ \\/_/\ \/\ \ \____\ \  __ \\ \___  \   //
//   \ \_\ \_\  \ \_\ \ \_____\\ \_\ \_\\/\_____\  //
//    \/_/\/_/   \/_/  \/_____/ \/_/\/_/ \/_____/  //
//                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/atlas.c

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/atlas.h"

#define RGBA_CHANNELS (4)

typedef struct
{
    int      rect[4];
    uint32_t buffer_index;
} atlas_node_t;

struct atlas_t
{
    size_t capacity;
    size_t count;

    size_t*        hashes;
    atlas_node_t** nodes;

    uint8_t* buffer;
    uint32_t buffer_index;

    uint8_t  expand;
    uint8_t  border;
    uint16_t width;
    uint16_t height;
    uint16_t resolution;
};

static int atlas_compare_area(const void* a, const void* b)
{
    int a_height = (*(atlas_node_t**)a)->rect[3];
    int b_height = (*(atlas_node_t**)b)->rect[3];

    return b_height - a_height;
}

static size_t atlas_generate_hash(uint8_t* pixels, size_t len)
{
    size_t hash = 5381;
    for (int i = 0; i < len; pixels++, ++i) hash = ((hash << 5) + hash) + (*pixels);
    return hash;
}

static bool atlas_check_hash_unique(size_t hash, size_t* hashes, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (hash == hashes[i]) return false;
    return true;
}

atlas_t* atlas_new(atlas_desc_t desc)
{
    assert(desc.capacity > 1);
    assert(desc.resolution > 4);

    atlas_t* atlas = (atlas_t*)malloc(sizeof(atlas_t));

    assert(atlas);

    atlas->expand     = desc.expand;
    atlas->border     = desc.border;
    atlas->resolution = desc.resolution;

    atlas->capacity = desc.capacity;
    atlas->count    = 0u;

    atlas->buffer_index = 0u;
    atlas->buffer       = (uint8_t*)malloc(desc.resolution * desc.resolution * RGBA_CHANNELS * sizeof(uint8_t));
    atlas->hashes       = (size_t*)malloc(sizeof(size_t) * desc.capacity);
    atlas->nodes        = (atlas_node_t**)malloc(sizeof(atlas_node_t*) * desc.capacity);

    assert(atlas->buffer);
    assert(atlas->hashes);
    assert(atlas->nodes);

    return atlas;
}

void atlas_delete(atlas_t* atlas)
{
    free(atlas->buffer);
    free(atlas->hashes);
    free(atlas->nodes);

    atlas->buffer = NULL;
    atlas->hashes = NULL;
    atlas->nodes  = NULL;

    free(atlas);
}

int (*atlas_add_texture(atlas_t* atlas, uint8_t* pixels, int width, int height))[4]
{
    size_t* size = &atlas->count;

    assert(*size < atlas->capacity);

    size_t* hashes         = atlas->hashes;
    size_t  hash           = atlas_generate_hash(pixels, width * height * RGBA_CHANNELS);
    bool    hash_is_unique = atlas_check_hash_unique(hash, hashes, *size);

    if (hash_is_unique)
    {
        atlas_node_t* node = (atlas_node_t*)malloc(sizeof(atlas_node_t));

        int rect[4] = {
            0,
            0,
            width,
            height,
        };

        if (node != NULL)
        {
            memcpy(node->rect, rect, 4 * sizeof(int));

            node->buffer_index = atlas->buffer_index;

            atlas->hashes[*size]    = hash;
            atlas->nodes[(*size)++] = node;

            uint32_t buffer_length = (uint32_t)(width * height * RGBA_CHANNELS);
            memcpy(atlas->buffer + atlas->buffer_index, pixels, buffer_length);
            atlas->buffer_index += buffer_length;
        }

        return &node->rect;
    }

    return NULL;
}

void atlas_pack(atlas_t* atlas)
{
    assert(atlas->count > 1);

    int area    = 0;
    int max_w   = atlas->nodes[0]->rect[2];
    int max_h   = atlas->nodes[0]->rect[3];
    int padding = atlas->expand * 2 + atlas->border;

    for (int i = 0; i < atlas->count; ++i)
    {
        while (max_w < atlas->nodes[i]->rect[2])
        {
            max_w *= 2;
            max_h = max_w;
        }

        area = max_w * max_h;
    }

    static const double suboptimal_coefficient = 0.85;

    assert(max_w <= atlas->resolution && max_h <= atlas->resolution);
    assert(area <= atlas->resolution * atlas->resolution * suboptimal_coefficient);

    const size_t len = atlas->count;

    int(*spaces)[4] = malloc(atlas->resolution * 4 * sizeof(int));

    int start_space[4] = {
        0,
        0,
        atlas->resolution,
        atlas->resolution,
    };

    memcpy(spaces[0], start_space, 4 * sizeof(int));

    qsort(atlas->nodes, len, sizeof(atlas_node_t*), atlas_compare_area);

    int n = 1;

    for (int i = 0; i < len; ++i)
    {
        atlas_node_t* node = atlas->nodes[i];

        int rect[4];

        memcpy(rect, node->rect, 4 * sizeof(int));

        assert(rect[2] <= atlas->resolution && rect[3] <= atlas->resolution);

        for (int j = n - 1; j >= 0; j--)
        {
            int space[4];

            memcpy(space, spaces[j], 4 * sizeof(int));

            int w = rect[2] + padding;
            int h = rect[3] + padding;

            // check if image too large for space
            if (w > space[2] || h > space[3]) continue;

            // add image to space's top-left
            // |-------|-------|
            // |  box  |       |
            // |_______|       |
            // |         space |
            // |_______________|

            rect[0] = space[0] + atlas->expand;
            rect[1] = space[1] + atlas->expand;

            while (max_w < rect[0] + rect[2])
            {
                max_w *= 2;
            }

            while (max_h < rect[1] + rect[3])
            {
                max_h *= 2;
            }

            if (w == space[2] && h == space[3])
            {
                // remove space if perfect fit
                // |---------------|
                // |               |
                // |      box      |
                // |               |
                // |_______________|

                int last[4];

                memcpy(last, spaces[--n], 4 * sizeof(int));

                if (j < n) memcpy(spaces[j], last, 4 * sizeof(int));
            }
            else if (h == space[3])
            {
                // space matches image height
                // move space right and cut off width
                // |-------|---------------|
                // |  box  | updated space |
                // |_______|_______________|

                spaces[j][0] += w;
                spaces[j][2] -= w;
            }
            else if (w == space[2])
            {
                // space matches image width
                // move space down and cut off height
                // |---------------|
                // |      box      |
                // |_______________|
                // | updated space |
                // |_______________|

                spaces[j][1] += h;
                spaces[j][3] -= h;
            }
            else
            {
                // split width and height
                // difference into two new spaces
                // |-------|-----------|
                // |  box  | new space |
                // |_______|___________|
                // | updated space     |
                // |___________________|

                int new_space[4] = {
                    space[0] + w,
                    space[1],
                    space[2] - w,
                    h,
                };

                memcpy(spaces[n++], new_space, 4 * sizeof(int));

                spaces[j][1] += h;
                spaces[j][3] -= h;
            }

            break;
        }

        memcpy(node->rect, rect, 4 * sizeof(int));
    }

    atlas->width  = max_w;
    atlas->height = max_h;
}

void atlas_generate_texture(atlas_t* atlas, uint8_t** pixels, int* width, int* height)
{
    size_t size = atlas->width * atlas->height * RGBA_CHANNELS * sizeof(uint8_t);

    *pixels = (uint8_t*)malloc(size);

    *width  = atlas->width;
    *height = atlas->height;

    memset(*pixels, 0, size);

    for (int i = 0; i < atlas->count; ++i)
    {
        const atlas_node_t* node = atlas->nodes[i];

        const int rect[4] = {
            node->rect[0],
            node->rect[1],
            node->rect[2],
            node->rect[3],
        };

        uint8_t* src_buffer = atlas->buffer + node->buffer_index;
        uint8_t* dst_buffer = *pixels;

        for (int y = -atlas->expand * RGBA_CHANNELS; y < (int)(rect[3] + atlas->expand) * RGBA_CHANNELS; y += RGBA_CHANNELS)
        {
            for (int x = -atlas->expand * RGBA_CHANNELS; x < (int)(rect[2] + atlas->expand) * RGBA_CHANNELS; x += RGBA_CHANNELS)
            {
                int src_x = x < 0 ? 0 : x > (rect[2] - 1) * RGBA_CHANNELS ? (rect[2] - 1) * RGBA_CHANNELS : x;
                int src_y = y < 0 ? 0 : y > (rect[3] - 1) * RGBA_CHANNELS ? (rect[3] - 1) * RGBA_CHANNELS : y;

                int dst_pixel_index = rect[0] * RGBA_CHANNELS + x + (rect[1] * RGBA_CHANNELS + y) * atlas->width;
                int src_pixel_index = src_x + src_y * rect[2];

                dst_buffer[dst_pixel_index + 0] = src_buffer[src_pixel_index + 0];
                dst_buffer[dst_pixel_index + 1] = src_buffer[src_pixel_index + 1];
                dst_buffer[dst_pixel_index + 2] = src_buffer[src_pixel_index + 2];
                dst_buffer[dst_pixel_index + 3] = src_buffer[src_pixel_index + 3];
            }
        }
    }
}
