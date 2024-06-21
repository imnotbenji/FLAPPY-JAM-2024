// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __       ______   ______     //
//  /\  ___\ /\  __ \ /\ \     /\  __ \ /\  == \    //
//  \ \ \____\ \ \/\ \\ \ \____\ \ \/\ \\ \  __<    //
//   \ \_____\\ \_____\\ \_____\\ \_____\\ \_\ \_\  //
//    \/_____/ \/_____/ \/_____/ \/_____/ \/_/ /_/  //
//                                                  //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/color.c

#include "graphics/color.h"

#define UINT8_MAX_FLOAT (255.0)

void rgb_from_hex(uint32_t hex, float rgb[3])
{
    rgb[0] = (uint8_t)((hex & 0xff0000) >> 16) / UINT8_MAX_FLOAT;
    rgb[1] = (uint8_t)((hex & 0x00ff00) >> 8) / UINT8_MAX_FLOAT;
    rgb[2] = (uint8_t)((hex & 0x0000ff)) / UINT8_MAX_FLOAT;
}