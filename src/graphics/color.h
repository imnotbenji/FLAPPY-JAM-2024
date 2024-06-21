// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __       ______   ______     //
//  /\  ___\ /\  __ \ /\ \     /\  __ \ /\  == \    //
//  \ \ \____\ \ \/\ \\ \ \____\ \ \/\ \\ \  __<    //
//   \ \_____\\ \_____\\ \_____\\ \_____\\ \_\ \_\  //
//    \/_____/ \/_____/ \/_____/ \/_____/ \/_/ /_/  //
//                                                  //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/color.h

#ifndef GRAPHICS_COLOR_H
#define GRAPHICS_COLOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define RGB_BLACK  (0x000000)
#define RGB_WHITE  (0xffffff)
#define RGB_RED    (0xff0000)
#define RGB_GREEN  (0x00ff00)
#define RGB_BLUE   (0x0000ff)
#define RGB_YELLOW (0xffff00)
#define RGB_PURPLE (0xff00ff)
#define RGB_CYAN   (0x00ffff)

    void rgb_from_hex(uint32_t hex, float rgb[3]);

#ifdef __cplusplus
}
#endif

#endif  // GRAPHICS_COLOR_H
