// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______  __  __   ______  __  __   ______   ______     //
//  /\__  _\/\  ___\/\_\_\_\ /\__  _\/\ \/\ \ /\  == \ /\  ___\    //
//  \/_/\ \/\ \  __\\/_/\_\/_\/_/\ \/\ \ \_\ \\ \  __< \ \  __\    //
//     \ \_\ \ \_____\/\_\/\_\  \ \_\ \ \_____\\ \_\ \_\\ \_____\  //
//      \/_/  \/_____/\/_/\/_/   \/_/  \/_____/ \/_/ /_/ \/_____/  //
//                                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/texture.c

#include <stddef.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"

#include "graphics/texture.h"

texture_t* texture_new_from_file(const char* path)
{
    texture_t* texture = (texture_t*)malloc(sizeof(texture_t));

    if (texture != NULL)
    {
        int width;
        int height;
        int bpp;

        uint8_t* pixels = stbi_load(path, &width, &height, &bpp, STBI_rgb_alpha);

        texture->pixels = pixels;
        texture->width  = width;
        texture->height = height;
    }

    return texture;
}

texture_t* texture_new_from_data(uint8_t* pixels, int width, int height)
{
    texture_t* texture = (texture_t*)malloc(sizeof(texture_t));

    if (texture != NULL)
    {
        texture->pixels = pixels;
        texture->width  = width;
        texture->height = height;
    }

    return texture;
}

texture_t* texture_new_empty(int width, int height)
{
    size_t size = width * height * STBI_rgb_alpha * sizeof(uint8_t);

    texture_t* texture = (texture_t*)malloc(sizeof(texture_t));
    uint8_t*   pixels  = (uint8_t*)malloc(size);

    if (texture != NULL && pixels != NULL)
    {
        memset(pixels, 0, size);

        texture->pixels = pixels;
        texture->width  = width;
        texture->height = height;
    }

    return texture;
}

void texture_delete(texture_t* texture) { free(texture->pixels); }

void texture_delete_stb(texture_t* texture) { stbi_image_free(texture->pixels); }

void texture_set_pixels(texture_t* texture, uint8_t* pixels, int dx, int dy, int width, int height)
{
    if (width > texture->width || height > texture->height) return;

    for (int y = 0; y < height; ++y)
    {
        int from = y * width;
        int to   = dx + (dy + y) * width;

        memcpy(pixels + to * STBI_rgb_alpha, pixels + from * STBI_rgb_alpha, width * STBI_rgb_alpha * sizeof(uint8_t));
    }
}
