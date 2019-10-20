#include "draw_util.h"

#include <stdio.h>

void draw_rect(TCOD_Console *console, int32_t x, int32_t y, int32_t width, int32_t height, char c, TCOD_color_t fore_color, TCOD_color_t back_color)
{
    for (int32_t y_offset = 0; y_offset < height; y_offset++)
    {
        for (int32_t x_offset = 0; x_offset < width; x_offset++)
        {
            if (x_offset == 0 || x_offset == width - 1 ||
                y_offset == 0 || y_offset == height - 1)
            {
                TCOD_console_put_char_ex(console, x + x_offset, y + y_offset, c, fore_color, back_color);
            }
        }   
    }
}

void swap_color(TCOD_Console *console, int32_t x, int32_t y)
{
    TCOD_color_t fore_color = TCOD_console_get_char_foreground(console, x, y);
    TCOD_color_t back_color = TCOD_console_get_char_background(console, x, y);

    TCOD_console_set_char_background(console, x, y, fore_color, TCOD_BKGND_SET);
    TCOD_console_set_char_foreground(console, x, y, back_color);
}

void xor_color(TCOD_Console *console, int32_t x, int32_t y, TCOD_color_t filter)
{
    TCOD_color_t fore_color = TCOD_console_get_char_foreground(console, x, y);
    TCOD_color_t back_color = TCOD_console_get_char_background(console, x, y);

    fore_color.r = fore_color.r ^ filter.r;
    fore_color.g = fore_color.g ^ filter.g;
    fore_color.b = fore_color.b ^ filter.b;

    back_color.r = back_color.r ^ filter.r;
    back_color.g = back_color.g ^ filter.g;
    back_color.b = back_color.b ^ filter.b;

    TCOD_console_set_char_background(console, x, y, back_color, TCOD_BKGND_SET);
    TCOD_console_set_char_foreground(console, x, y, fore_color);
}