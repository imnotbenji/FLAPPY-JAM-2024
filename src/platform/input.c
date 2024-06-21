// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __   __   __   ______  __  __   ______   //
//  /\ \ /\ "-.\ \ /\  == \/\ \/\ \ /\__  _\  //
//  \ \ \\ \ \-.  \\ \  _-/\ \ \_\ \\/_/\ \/  //
//   \ \_\\ \_\\"\_\\ \_\   \ \_____\  \ \_\  //
//    \/_/ \/_/ \/_/ \/_/    \/_____/   \/_/  //
//                                            //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/input.c

#include <string.h>

#include "platform/input.h"
#include "platform/platform.h"

#define NUM_SCANCODES  (512)

#define MAX_NUM_KEYS   (512)
#define MAX_NUM_CLICKS (16)

static struct
{
    uint8_t prev_keyboard_state[NUM_SCANCODES];
    uint8_t curr_keyboard_state[NUM_SCANCODES];
} keyboard;

static struct
{
    uint32_t prev_mouse_state;
    uint32_t curr_mouse_state;
    int      prev_mouse_x;
    int      prev_mouse_y;
    int      curr_mouse_x;
    int      curr_mouse_y;
} mouse;

void input_update(void)
{
    memcpy(keyboard.prev_keyboard_state, keyboard.curr_keyboard_state, sizeof(uint8_t) * NUM_SCANCODES);
    memcpy(keyboard.curr_keyboard_state, platform_get_keyboard_state(), sizeof(uint8_t) * NUM_SCANCODES);

    mouse.prev_mouse_x     = mouse.curr_mouse_x;
    mouse.prev_mouse_y     = mouse.curr_mouse_y;
    mouse.prev_mouse_state = mouse.curr_mouse_state;
    mouse.curr_mouse_state = platform_get_mouse_state(&mouse.curr_mouse_x, &mouse.curr_mouse_y);
}

bool input_key_up(uint8_t key) { return !keyboard.curr_keyboard_state[key] && keyboard.prev_keyboard_state[key]; }

bool input_key_down(uint8_t key) { return keyboard.curr_keyboard_state[key]; }

bool input_key_pressed(uint8_t key) { return keyboard.curr_keyboard_state[key] && !keyboard.prev_keyboard_state[key]; }

bool input_mouse_up(uint32_t button) { return ((MOUSE(button) & mouse.curr_mouse_state) == 0) && ((MOUSE(button) & mouse.prev_mouse_state) != 0); }

bool input_mouse_down(uint32_t button) { return (MOUSE(button) & mouse.curr_mouse_state) != 0; }

bool input_mouse_click(uint32_t button) { return ((MOUSE(button) & mouse.curr_mouse_state) != 0) && ((MOUSE(button) & mouse.prev_mouse_state) == 0); }

void input_mouse_get_position(int* x, int* y)
{
    *x = mouse.curr_mouse_x;
    *y = mouse.curr_mouse_y;
}
