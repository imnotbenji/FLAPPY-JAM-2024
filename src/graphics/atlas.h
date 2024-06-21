// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______  __       ______   ______     //
//  /\  __ \ /\__  _\/\ \     /\  __ \ /\  ___\    //
//  \ \  __ \\/_/\ \/\ \ \____\ \  __ \\ \___  \   //
//   \ \_\ \_\  \ \_\ \ \_____\\ \_\ \_\\/\_____\  //
//    \/_/\/_/   \/_/  \/_____/ \/_/\/_/ \/_____/  //
//                                                 //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/atlas.h

#ifndef GRAPHICS_ATLAS_H
#define GRAPHICS_ATLAS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct texture_t texture_t;
    typedef struct atlas_t   atlas_t;

    typedef struct atlas_desc_t
    {
        size_t   capacity;
        uint8_t  expand;
        uint8_t  border;
        uint16_t resolution;
    } atlas_desc_t;

    atlas_t* atlas_new(atlas_desc_t desc);
    void     atlas_delete(atlas_t* atlas);

    int (*atlas_add_texture(atlas_t* atlas, uint8_t* pixels, int width, int height))[4];

    void atlas_pack(atlas_t* atlas);
    void atlas_generate_texture(atlas_t* atlas, uint8_t** pixels, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_ATLAS_H
