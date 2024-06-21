// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______  __  __   ______  __  __   ______   ______     //
//  /\__  _\/\  ___\/\_\_\_\ /\__  _\/\ \/\ \ /\  == \ /\  ___\    //
//  \/_/\ \/\ \  __\\/_/\_\/_\/_/\ \/\ \ \_\ \\ \  __< \ \  __\    //
//     \ \_\ \ \_____\/\_\/\_\  \ \_\ \ \_____\\ \_\ \_\\ \_____\  //
//      \/_/  \/_____/\/_/\/_/   \/_/  \/_____/ \/_/ /_/ \/_____/  //
//                                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/texture.h

#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct texture_t
    {
        int      width;
        int      height;
        uint8_t* pixels;
    } texture_t;

    texture_t* texture_new_from_file(const char* path);
    texture_t* texture_new_from_data(uint8_t* pixels, int width, int height);
    texture_t* texture_new_empty(int width, int height);
    void       texture_delete(texture_t* texture);
    void       texture_delete_stb(texture_t* texture);

    void texture_set_pixels(texture_t* texture, uint8_t* pixels, int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_TEXTURE_H
