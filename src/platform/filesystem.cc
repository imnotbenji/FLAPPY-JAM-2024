// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  __   __       ______   ______   __  __   ______   ______  ______   __    __     //
//  /\  ___\/\ \ /\ \     /\  ___\ /\  ___\ /\ \_\ \ /\  ___\ /\__  _\/\  ___\ /\ "-./  \    //
//  \ \  __\\ \ \\ \ \____\ \  __\ \ \___  \\ \____ \\ \___  \\/_/\ \/\ \  __\ \ \ \-./\ \   //
//   \ \_\   \ \_\\ \_____\\ \_____\\/\_____\\/\_____\\/\_____\  \ \_\ \ \_____\\ \_\ \ \_\  //
//    \/_/    \/_/ \/_____/ \/_____/ \/_____/ \/_____/ \/_____/   \/_/  \/_____/ \/_/  \/_/  //
//                                                                                           //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/filesystem.c

#include <cstring>
#include <filesystem>
#include <vector>

#include "platform/filesystem.h"

char* filesystem_get_file_name(const char* path, bool with_extension)
{
    auto file   = std::filesystem::path(path).filename();
    auto result = with_extension ? file.string() : file.replace_extension().string();

    char* buffer = new char[result.size() + 1];
    strncpy(buffer, result.c_str(), result.size() + 1);

    return buffer;
}

char* filesystem_get_file_extension(const char* path)
{
    auto result = std::filesystem::path(path).extension().string();

    char* buffer = new char[result.size() + 1];
    strncpy(buffer, result.c_str(), result.size() + 1);

    return buffer;
}

void filesystem_enumerate_dir(const char* path, const char*** list, size_t* len, bool recursive)
{
    if (std::filesystem::is_directory(std::filesystem::status(path)))
    {
        std::vector<std::string> files;

        if (recursive)
        {
            for (auto& p : std::filesystem::recursive_directory_iterator(path)) files.emplace_back(p.path().string());
        }
        else
        {
            for (auto& p : std::filesystem::directory_iterator(path)) files.emplace_back(p.path().string());
        }

        *len  = files.size();
        *list = new const char*[files.size()];

        for (int i = 0; i < files.size(); ++i)
        {
            (*list)[i] = new char[files[i].size() + 1];
            strncpy((char*)(*list)[i], files[i].c_str(), files[i].size() + 1);
        }
    }
}

void filesystem_free_file_list(const char** list, size_t len)
{
    for (int i = 0; i < len; ++i)
    {
        delete[] list[i];
    }

    delete[] list;
}
