// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __  __   ______   _____    ______   ______     //
//  /\  ___\ /\ \_\ \ /\  __ \ /\  __-. /\  ___\ /\  == \    //
//  \ \___  \\ \  __ \\ \  __ \\ \ \/\ \\ \  __\ \ \  __<    //
//   \/\_____\\ \_\ \_\\ \_\ \_\\ \____- \ \_____\\ \_\ \_\  //
//    \/_____/ \/_/\/_/ \/_/\/_/ \/____/  \/_____/ \/_/ /_/  //
//                                                           //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/shader.h

#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <stddef.h>
#include <stdint.h>

#define SHADER_MAX_UNIFORMS     (256)
#define SHADER_MAX_UNIFORM_NAME (32)

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct shader_t
    {
        uint32_t id;
        size_t   uniforms_len;
        struct
        {
            int  location;
            char name[SHADER_MAX_UNIFORM_NAME];
        } uniforms[SHADER_MAX_UNIFORMS];
    } shader_t;

    shader_t* shader_new(const char* filepath);
    void      shader_delete(shader_t* shader);

    void shader_apply_uniformi(shader_t* shader, const char* name, int value);
    void shader_apply_uniformf(shader_t* shader, const char* name, float value);
    void shader_apply_uniform2f(shader_t* shader, const char* name, float* value);
    void shader_apply_uniform3f(shader_t* shader, const char* name, float* value);
    void shader_apply_uniform4f(shader_t* shader, const char* name, float* value);
    void shader_apply_uniformfv(shader_t* shader, const char* name, size_t len, float* value);
    void shader_apply_uniform4x4f(shader_t* shader, const char* name, float* value);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_SHADER_H
