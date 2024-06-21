// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __   __   ______  ______   __   __   ______   //
//  /\  ___\ /\  __ \ /\ "-.\ \ /\__  _\/\  ___\ /\ "-.\ \ /\__  _\  //
//  \ \ \____\ \ \/\ \\ \ \-.  \\/_/\ \/\ \  __\ \ \ \-.  \\/_/\ \/  //
//   \ \_____\\ \_____\\ \_\\"\_\  \ \_\ \ \_____\\ \_\\"\_\  \ \_\  //
//    \/_____/ \/_____/ \/_/ \/_/   \/_/  \/_____/ \/_/ \/_/   \/_/  //
//                                                                   //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/content.c

#include <cstring>
#include <string>
#include <unordered_map>

#include "platform/content.h"
#include "platform/platform.h"
#include "platform/filesystem.h"

#include "graphics/shader.h"
#include "graphics/atlas.h"
#include "graphics/texture.h"

#include "audio/sound.h"

using file_action_func = void(const char* filename, const char* filepath, void* data);

static void content_load_asset(const char* path, const char* extension, void* data, file_action_func action);

#define CONTENT_DEFINE(type, extension, path, name)                                        \
    static std::unordered_map<std::string, type> name;                                     \
                                                                                           \
    static void asset_load_##name(const char* filename, const char* filepath, void* data); \
    static void asset_unload_##name(type& asset);                                          \
                                                                                           \
    void content_load_##name(void)                                                         \
    {                                                                                      \
        name.clear();                                                                      \
        content_load_asset(path, extension, NULL, asset_load_##name);                      \
    }                                                                                      \
                                                                                           \
    void content_load_##name##_ex(void* data)                                              \
    {                                                                                      \
        name.clear();                                                                      \
        content_load_asset(path, extension, data, asset_load_##name);                      \
    }                                                                                      \
                                                                                           \
    void content_unload_##name(void)                                                       \
    {                                                                                      \
        for (auto& [key, value] : name)                                                    \
        {                                                                                  \
            asset_unload_##name(value);                                                    \
        }                                                                                  \
        name.clear();                                                                      \
    }                                                                                      \
                                                                                           \
    bool content_find_##name(const char* filename, type* asset)                            \
    {                                                                                      \
        if (name.find(std::string(filename)) != name.end())                                \
        {                                                                                  \
            *asset = name[std::string(filename)];                                          \
            return true;                                                                   \
        }                                                                                  \
        return false;                                                                      \
    }

#define X(type, extension, path, name) CONTENT_DEFINE(type, extension, path, name)
CONTENT_TYPES
#undef X

static void content_load_asset(const char* path, const char* extension, void* data, file_action_func action)
{
    size_t       dir_len   = 0;
    const char** dir_files = NULL;

    filesystem_enumerate_dir((std::string(platform_get_path()) + "/assets/" + path).c_str(), &dir_files, &dir_len, true);

    for (int i = 0; i < dir_len; ++i)
    {
        const char* filepath       = dir_files[i];
        const char* filename       = filesystem_get_file_name(filepath, false);
        const char* file_extension = filesystem_get_file_extension(filepath);

        bool is_correct_extension = false;

        const char* ext_start = extension;

        while (*ext_start)
        {
            const char* ext_end = strchr(ext_start, '|');
            if (!ext_end) ext_end = ext_start + strlen(ext_start);

            if (strncmp(file_extension, ext_start, ext_end - ext_start) == 0 && file_extension[ext_end - ext_start] == '\0')
            {
                is_correct_extension = true;
                break;
            }

            ext_start = *ext_end ? ext_end + 1 : ext_end;
        }

        if (is_correct_extension)
        {
            action(filename, filepath, data);
#ifdef DEBUG
            printf("   - Loaded \"%s\"\n", filepath);
#endif
        }

        free((char*)filename);
        free((char*)file_extension);
    }

    filesystem_free_file_list(dir_files, dir_len);
}

static void asset_load_shaders(const char* filename, const char* filepath, void* data)
{
    shader_t* shader = shader_new(filepath);

    shaders.insert({std::string(filename), shader});
}

static void asset_load_textures(const char* filename, const char* filepath, void* data)
{
    atlas_t* atlas = (atlas_t*)data;

    texture_t* texture = texture_new_from_file(filepath);

    int(*src)[4] = atlas_add_texture(atlas, texture->pixels, texture->width, texture->height);

    textures.insert({std::string(filename), src});

    texture_delete_stb(texture);
}

static void asset_load_sounds(const char* filename, const char* filepath, void* data)
{
    sound_t* sound = sound_new(filepath);

    sounds.insert({std::string(filename), sound});
}

static void asset_unload_shaders(shader_t*& shader) { shader_delete(shader); }

static void asset_unload_textures(texture_src_t*& texture) {}

static void asset_unload_sounds(sound_t*& sound) { sound_delete(sound); }
