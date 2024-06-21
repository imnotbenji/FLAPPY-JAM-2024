// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __   __   ______  ______   __   __   ______   //
//  /\  ___\ /\  __ \ /\ "-.\ \ /\__  _\/\  ___\ /\ "-.\ \ /\__  _\  //
//  \ \ \____\ \ \/\ \\ \ \-.  \\/_/\ \/\ \  __\ \ \ \-.  \\/_/\ \/  //
//   \ \_____\\ \_____\\ \_\\"\_\  \ \_\ \ \_____\\ \_\\"\_\  \ \_\  //
//    \/_____/ \/_____/ \/_/ \/_/   \/_/  \/_____/ \/_/ \/_/   \/_/  //
//                                                                   //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/content.h

#ifndef PLATFORM_CONTENT_H
#define PLATFORM_CONTENT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct shader_t shader_t;
    typedef struct sound_t  sound_t;
    typedef int             texture_src_t[4];

#define CONTENT_DECLARE(type, name)            \
    void content_load_##name(void);            \
    void content_load_##name##_ex(void* data); \
    void content_unload_##name(void);          \
    bool content_find_##name(const char* filename, type* asset);

#define CONTENT_TYPES                           \
    X(shader_t*, ".shader", "shaders", shaders) \
    X(sound_t*, ".wav", "sounds", sounds)       \
    X(texture_src_t*, ".png|.jpg|.jpeg", "textures", textures)

#define X(type, extension, path, name) CONTENT_DECLARE(type, name)
    CONTENT_TYPES
#undef X

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_CONTENT_H
