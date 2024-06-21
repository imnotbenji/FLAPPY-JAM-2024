// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   __  __   ______   _____    ______   ______     //
//  /\  ___\ /\ \_\ \ /\  __ \ /\  __-. /\  ___\ /\  == \    //
//  \ \___  \\ \  __ \\ \  __ \\ \ \/\ \\ \  __\ \ \  __<    //
//   \/\_____\\ \_\ \_\\ \_\ \_\\ \____- \ \_____\\ \_\ \_\  //
//    \/_____/ \/_/\/_/ \/_/\/_/ \/____/  \/_____/ \/_/ /_/  //
//                                                           //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/shader.c

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/shader.h"
#include "graphics/renderer.h"

#define SHADER_MAX_LINE_LENGTH (4096)
#define SHADER_MAX_LINE_COUNT  (8192)

#define VERTEX_SHADER_TAG      ("@vs")
#define FRAGMENT_SHADER_TAG    ("@fs")

static void prepend_precision(char* shader)
{
#ifdef __EMSCRIPTEN__
    strcat(shader, "precision mediump float;");
    strcat(shader, "\n");
#endif
}

static void prepend_version(char* shader)
{
#ifdef __EMSCRIPTEN__
    strcat(shader, "#version 300 es");
#else
    strcat(shader, "#version 330 core");
#endif
    strcat(shader, "\n");
}

shader_t* shader_new(const char* filepath)
{
    shader_t* shader = (shader_t*)malloc(sizeof(shader_t));

    if (shader != NULL)
    {
        char  line[SHADER_MAX_LINE_LENGTH];
        char  vs[SHADER_MAX_LINE_COUNT] = "";
        char  fs[SHADER_MAX_LINE_COUNT] = "";
        char* curr_shader               = NULL;

        FILE* file = fopen(filepath, "rb");

        if (!file)
        {
            free(shader);
            return NULL;
        }

        shader->uniforms_len = 0U;

        while (fgets(line, sizeof(line), file))
        {
            if (strstr(line, VERTEX_SHADER_TAG))
            {
                curr_shader = vs;

                prepend_version(curr_shader);

                continue;
            }

            if (strstr(line, FRAGMENT_SHADER_TAG))
            {
                curr_shader = fs;

                prepend_version(curr_shader);
                prepend_precision(curr_shader);

                continue;
            }

            if (curr_shader)
            {
                strcat(curr_shader, line);
            }
        }

        shader->id = renderer_shader_generate(vs, fs);

        for (int i = 0; i < shader->uniforms_len; ++i)
        {
            int location = renderer_shader_get_uniform_location(shader->id, shader->uniforms[i].name);

            shader->uniforms[i].location = location;
        }

        fclose(file);
    }

    return shader;
}

void shader_delete(shader_t* shader)
{
    renderer_shader_delete(shader->id);
    free(shader);
}

static int get_cached_uniform_location(shader_t* shader, const char* name)
{
    for (int i = 0; i < shader->uniforms_len; ++i)
    {
        if (strcmp(shader->uniforms[i].name, name) == 0)
        {
            return shader->uniforms[i].location;
        }
    }

    size_t uniforms_len = shader->uniforms_len;
    assert(uniforms_len < SHADER_MAX_UNIFORMS);

    int location = renderer_shader_get_uniform_location(shader->id, name);

    assert(location > -1);

    shader->uniforms[uniforms_len].location = location;
    strcpy(shader->uniforms[uniforms_len].name, name);
    ++shader->uniforms_len;

    return location;
}

void shader_apply_uniformi(shader_t* shader, const char* name, int value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniformi(location, value);
}

void shader_apply_uniformf(shader_t* shader, const char* name, float value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniformf(location, value);
}

void shader_apply_uniform2f(shader_t* shader, const char* name, float* value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniform2f(location, value);
}

void shader_apply_uniform3f(shader_t* shader, const char* name, float* value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniform3f(location, value);
}

void shader_apply_uniform4f(shader_t* shader, const char* name, float* value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniform4f(location, value);
}

void shader_apply_uniformfv(shader_t* shader, const char* name, size_t len, float* value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniformfv(location, len, value);
}

void shader_apply_uniform4x4f(shader_t* shader, const char* name, float* value)
{
    int location = get_cached_uniform_location(shader, name);
    renderer_shader_set_uniform4x4f(location, value);
}
