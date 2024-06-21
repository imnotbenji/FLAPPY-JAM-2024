// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  __       ______   ______  ______  ______   ______   __    __     //
//  /\  == \/\ \     /\  __ \ /\__  _\/\  ___\/\  __ \ /\  == \ /\ "-./  \    //
//  \ \  _-/\ \ \____\ \  __ \\/_/\ \/\ \  __\\ \ \/\ \\ \  __< \ \ \-./\ \   //
//   \ \_\   \ \_____\\ \_\ \_\  \ \_\ \ \_\   \ \_____\\ \_\ \_\\ \_\ \ \_\  //
//    \/_/    \/_____/ \/_/\/_/   \/_/  \/_/    \/_____/ \/_/ /_/ \/_/  \/_/  //
//                                                                            //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/platform.h

#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        EVENT_NONE   = 0x0,
        EVENT_QUIT   = 0x1,
        EVENT_RESIZE = 0x2,
    } EVENT;

    void* platform_create_window(const char* title, int width, int height);
    void* platform_create_context(void* window);

    void platform_shutdown(void);

    void  platform_swap_window(void);
    void* platform_get_function(void);

    bool platform_events(uint8_t* event);

    uint32_t platform_get_mouse_state(int* x, int* y);
    uint8_t* platform_get_keyboard_state(void);

    uint64_t platform_get_ticks(void);
    uint64_t platform_get_ticks_frequency(void);

    void platform_get_window_size(int* width, int* height);
    void platform_fullscreen_enable(void);
    void platform_fullscreen_disable(void);
    bool platform_is_window_active(void);
    bool platform_is_window_fullscreen(void);

    void platform_vsync_enable(void);
    void platform_vsync_disable(void);

    const char* platform_get_path(void);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_H
