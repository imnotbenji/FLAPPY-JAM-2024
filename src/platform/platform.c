// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  __       ______   ______  ______  ______   ______   __    __     //
//  /\  == \/\ \     /\  __ \ /\__  _\/\  ___\/\  __ \ /\  == \ /\ "-./  \    //
//  \ \  _-/\ \ \____\ \  __ \\/_/\ \/\ \  __\\ \ \/\ \\ \  __< \ \ \-./\ \   //
//   \ \_\   \ \_____\\ \_\ \_\  \ \_\ \ \_\   \ \_____\\ \_\ \_\\ \_\ \ \_\  //
//    \/_/    \/_____/ \/_/\/_/   \/_/  \/_/    \/_____/ \/_/ /_/ \/_/  \/_/  //
//                                                                            //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/platform.c

#include <assert.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif
#include "SDL.h"

#include "platform/platform.h"

#ifdef DEBUG
#define GL_VENDOR                   (0x1F00)
#define GL_RENDERER                 (0x1F01)
#define GL_VERSION                  (0x1F02)
#define GL_SHADING_LANGUAGE_VERSION (0x8B8C)

typedef unsigned int  GLenum;
typedef unsigned char GLubyte;
typedef const GLubyte* (*glGetString_t)(GLenum name);
#endif

static SDL_Window*    window;
static SDL_GLContext* glcontext;

void* platform_create_window(const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        return NULL;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

#ifdef DEBUG
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    printf("★ Created SDL2 Window\n");
    printf("SDL compiled version:\t%d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
    printf("SDL linked version:\t%d.%d.%d\n\n", linked.major, linked.minor, linked.patch);
#endif

    return (void*)window;
}

void* platform_create_context(void* window)
{
#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    SDL_GLContext glcontext = SDL_GL_CreateContext((SDL_Window*)window);

#ifdef DEBUG
    glGetString_t glGetString = (glGetString_t)SDL_GL_GetProcAddress("glGetString");

    assert(glGetString);

    const GLubyte* vendor       = glGetString(GL_VENDOR);
    const GLubyte* renderer     = glGetString(GL_RENDERER);
    const GLubyte* version      = glGetString(GL_VERSION);
    const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("★ Created OpenGL context\n");
    printf("GL vendor:\t\t%s\n", vendor);
    printf("GL renderer:\t\t%s\n", renderer);
    printf("GL version:\t\t%s\n", version);
    printf("GLSL version:\t\t%s\n\n", glsl_version);
#endif

    SDL_GL_MakeCurrent((SDL_Window*)window, (SDL_GLContext)glcontext);

    return (void*)glcontext;
}

void platform_shutdown(void)
{
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void platform_swap_window(void) { SDL_GL_SwapWindow(window); }

void* platform_get_function(void) { return (void*)&SDL_GL_GetProcAddress; }

bool platform_events(uint8_t* event)
{
    static SDL_Event e;

    bool has_event = SDL_PollEvent(&e);

    switch (e.type)
    {
        case SDL_QUIT: *event = EVENT_QUIT; break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_CLOSE) *event = EVENT_QUIT;
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) *event = EVENT_RESIZE;
            break;

        default: break;
    }

    return has_event;
}

uint32_t platform_get_mouse_state(int* x, int* y) { return SDL_GetMouseState(x, y); }

uint8_t* platform_get_keyboard_state(void) { return (uint8_t*)SDL_GetKeyboardState(NULL); }

uint64_t platform_get_ticks(void) { return SDL_GetPerformanceCounter(); }

uint64_t platform_get_ticks_frequency(void) { return SDL_GetPerformanceFrequency(); }

void platform_get_window_size(int* width, int* height)
{
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_get_current_context();
    emscripten_webgl_get_drawing_buffer_size(context, width, height);
#else
    SDL_GL_GetDrawableSize(window, width, height);
#endif
}

void platform_fullscreen_enable(void) { SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); }

void platform_fullscreen_disable(void) { SDL_SetWindowFullscreen(window, SDL_FALSE); }

bool platform_is_window_active(void)
{
    uint32_t flags = SDL_GetWindowFlags(window);
    return (flags & SDL_WINDOW_INPUT_FOCUS) != 0 && (flags & SDL_WINDOW_MINIMIZED) == 0;
}

bool platform_is_window_fullscreen(void) { return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP; }

void platform_vsync_enable(void) { SDL_GL_SetSwapInterval(1); }

void platform_vsync_disable(void) { SDL_GL_SetSwapInterval(0); }

const char* platform_get_path(void) { return SDL_GetBasePath(); }
