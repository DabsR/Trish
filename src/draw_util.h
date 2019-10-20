#ifndef DRAW_UTIL_H
#define DRAW_UTIL_H 1

#include <stdint.h>
#include <libtcod.h>



void draw_rect(TCOD_Console *console, int32_t x, int32_t y, int32_t width, int32_t height, char c, TCOD_color_t fore_color, TCOD_color_t back_color);

void swap_color(TCOD_Console *console, int32_t x, int32_t y);
void xor_color(TCOD_Console *console, int32_t x, int32_t y, TCOD_color_t filter);

#endif