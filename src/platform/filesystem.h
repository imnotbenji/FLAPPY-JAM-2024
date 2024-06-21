// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  __   __       ______   ______   __  __   ______   ______  ______   __    __     //
//  /\  ___\/\ \ /\ \     /\  ___\ /\  ___\ /\ \_\ \ /\  ___\ /\__  _\/\  ___\ /\ "-./  \    //
//  \ \  __\\ \ \\ \ \____\ \  __\ \ \___  \\ \____ \\ \___  \\/_/\ \/\ \  __\ \ \ \-./\ \   //
//   \ \_\   \ \_\\ \_____\\ \_____\\/\_____\\/\_____\\/\_____\  \ \_\ \ \_____\\ \_\ \ \_\  //
//    \/_/    \/_/ \/_____/ \/_____/ \/_____/ \/_____/ \/_____/   \/_/  \/_____/ \/_/  \/_/  //
//                                                                                           //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/filesystem.h

#ifndef PLATFORM_FILESYSTEM_H
#define PLATFORM_FILESYSTEM_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    char* filesystem_get_file_name(const char* path, bool with_extension);
    char* filesystem_get_file_extension(const char* path);

    void filesystem_enumerate_dir(const char* path, const char*** list, size_t* len, bool recursive);
    void filesystem_free_file_list(const char** list, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_FILESYSTEM_H
