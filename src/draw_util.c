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