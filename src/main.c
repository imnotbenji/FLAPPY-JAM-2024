// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __    __   ______   __   __   __     //
//  /\ "-./  \ /\  __ \ /\ \ /\ "-.\ \    //
//  \ \ \-./\ \\ \  __ \\ \ \\ \ \-.  \   //
//   \ \_\ \ \_\\ \_\ \_\\ \_\\ \_\\"\_\  //
//    \/_/  \/_/ \/_/\/_/ \/_/ \/_/ \/_/  //
//                                        //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// main.c

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "game/game.h"

#include "platform/input.h"
#include "platform/platform.h"

#include "graphics/renderer.h"

#define WIDTH      (720)
#define HEIGHT     (960)

#define TARGET_FPS (60)

static uint64_t start_time;
static uint64_t prev_time;
static uint64_t curr_time;
static double   frame_time;
static double   delta_time;
static double   total_time;
static double   accumulator;

static void app_quit(void);
static void app_resize(void);

static void app_init(void* window, void* context)
{
    srand(time(NULL));

#ifndef __EMSCRIPTEN__
    platform_vsync_enable();
#endif

    game_init();
}

static void app_events(uint8_t event)
{
    switch (event)
    {
        case EVENT_QUIT: app_quit(); break;
        case EVENT_RESIZE: app_resize(); break;

        default: break;
    }
}

static void app_update(void)
{
    input_update();

    game_update(1 / (float)TARGET_FPS, total_time);

    if (input_key_down(KEY_LEFTALT) && input_key_pressed(KEY_ENTER))
    {
        if (!platform_is_window_fullscreen()) platform_fullscreen_enable();
        else platform_fullscreen_disable();
    }
}

static void app_render(void) { game_render(delta_time, total_time); }

static void app_shutdown(void) { game_shutdown(); }

static void app_step(void)
{
    uint64_t ticks     = platform_get_ticks();
    double   frequency = 1.0 / (double)platform_get_ticks_frequency();

    prev_time  = curr_time;
    curr_time  = ticks;
    total_time = (curr_time - start_time) * frequency;
    delta_time = (double)((curr_time - prev_time) * frequency);

    if (delta_time > 1.0)
    {
        delta_time = 1.0;
    }

    accumulator += delta_time;

    uint8_t event = 0u;

    while (platform_events(&event))
    {
        app_events(event);
    }

    while (accumulator >= frame_time)
    {
        app_update();

        accumulator -= frame_time;
    }

    app_render();
    platform_swap_window();
}

#ifdef __EMSCRIPTEN__
static void app_quit(void) { emscripten_cancel_main_loop(); }
#else
static bool app_is_running = true;
static void app_quit(void) { app_is_running = false; }
#endif

static void app_resize(void)
{
    int width, height;

    platform_get_window_size(&width, &height);
    renderer_viewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    void* window  = platform_create_window("game", WIDTH, HEIGHT);
    void* context = platform_create_context(window);

    assert(window);
    assert(context);

    renderer_bind((void* (*)(const char*))platform_get_function());

    app_init(window, context);

    frame_time  = 1 / (double)TARGET_FPS;
    start_time  = platform_get_ticks();
    curr_time   = start_time;
    prev_time   = 0;
    accumulator = 0;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(app_step, 0, 1);
#else
    while (app_is_running) app_step();
#endif

    app_shutdown();
    platform_shutdown();

    return EXIT_SUCCESS;
}
