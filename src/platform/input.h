// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   __   __   __   ______  __  __   ______   //
//  /\ \ /\ "-.\ \ /\  == \/\ \/\ \ /\__  _\  //
//  \ \ \\ \ \-.  \\ \  _-/\ \ \_\ \\/_/\ \/  //
//   \ \_\\ \_\\"\_\\ \_\   \ \_____\  \ \_\  //
//    \/_/ \/_/ \/_/ \/_/    \/_____/   \/_/  //
//                                            //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// platform/input.h

#ifndef PLATFORM_INPUT_H
#define PLATFORM_INPUT_H

#define KEY_A            (4)
#define KEY_B            (5)
#define KEY_C            (6)
#define KEY_D            (7)
#define KEY_E            (8)
#define KEY_F            (9)
#define KEY_G            (10)
#define KEY_H            (11)
#define KEY_I            (12)
#define KEY_J            (13)
#define KEY_K            (14)
#define KEY_L            (15)
#define KEY_M            (16)
#define KEY_N            (17)
#define KEY_O            (18)
#define KEY_P            (19)
#define KEY_Q            (20)
#define KEY_R            (21)
#define KEY_S            (22)
#define KEY_T            (23)
#define KEY_U            (24)
#define KEY_V            (25)
#define KEY_W            (26)
#define KEY_X            (27)
#define KEY_Y            (28)
#define KEY_Z            (29)

#define KEY_NUM1         (30)
#define KEY_NUM2         (31)
#define KEY_NUM3         (32)
#define KEY_NUM4         (33)
#define KEY_NUM5         (34)
#define KEY_NUM6         (35)
#define KEY_NUM7         (36)
#define KEY_NUM8         (37)
#define KEY_NUM9         (38)
#define KEY_NUM0         (39)

#define KEY_ENTER        (40)
#define KEY_ESCAPE       (41)
#define KEY_BACKSPACE    (42)
#define KEY_TAB          (43)
#define KEY_SPACE        (44)

#define KEY_MINUS        (45)
#define KEY_EQUALS       (46)
#define KEY_LEFTBRACKET  (47)
#define KEY_RIGHTBRACKET (48)
#define KEY_BACKSLASH    (49)
#define KEY_SEMICOLON    (51)
#define KEY_APOSTROPHE   (52)
#define KEY_TILDE        (53)
#define KEY_COMMA        (54)
#define KEY_PERIOD       (55)
#define KEY_SLASH        (56)
#define KEY_CAPSLOCK     (57)

#define KEY_F1           (58)
#define KEY_F2           (59)
#define KEY_F3           (60)
#define KEY_F4           (61)
#define KEY_F5           (62)
#define KEY_F6           (63)
#define KEY_F7           (64)
#define KEY_F8           (65)
#define KEY_F9           (66)
#define KEY_F10          (67)
#define KEY_F11          (68)
#define KEY_F12          (69)

#define KEY_INSERT       (73)
#define KEY_HOME         (74)
#define KEY_DELETE       (76)
#define KEY_END          (77)
#define KEY_RIGHT        (79)
#define KEY_LEFT         (80)
#define KEY_DOWN         (81)
#define KEY_UP           (82)

#define KEY_LEFTCONTROL  (224)
#define KEY_LEFTSHIFT    (225)
#define KEY_LEFTALT      (226)
#define KEY_LEFTOS       (227)
#define KEY_RIGHTCONTROL (228)
#define KEY_RIGHTSHIFT   (229)
#define KEY_RIGHTALT     (230)
#define KEY_RIGHTOS      (231)

#define MOUSE(X)         (1 << ((X) - 1))
#define MOUSE_LEFT       (1)
#define MOUSE_MIDDLE     (2)
#define MOUSE_RIGHT      (3)

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void input_update(void);

    bool input_key_up(uint8_t key);
    bool input_key_down(uint8_t key);
    bool input_key_pressed(uint8_t key);

    bool input_mouse_up(uint32_t button);
    bool input_mouse_down(uint32_t button);
    bool input_mouse_click(uint32_t button);

    void input_mouse_get_position(int* x, int* y);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_INPUT_H
