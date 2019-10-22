#include "render.h"

#include <stdio.h>
#include <libtcod.h>

#include "tile.h"
#include "map.h"
#include "view.h"
#include "monster.h"
#include "player.h"



void render_events(EventView *view)
{
    // @Incomplete Render event log here.
}

void render_map(Map *map, MapView *view)
{
    // @Incomplete: We need to account
    // for map scrolling.

    for (size_t y = 0; y < map->height; y++)
    {
        for (size_t x = 0; x < map->width; x++)
        {
            Tile *tile     = &map->tiles[x + y * map->width];
            int32_t draw_x = view->x + x;
            int32_t draw_y = view->y + y;

            if (!tile->type_info->is_visible) continue;

            // The background of the tile is drawn regardless
            // of what is present on the tile.
            TCOD_console_set_char_background(0, draw_x, draw_y, tile->type_info->back_color, 0);
            
            // Entities such as the player, monsters, and items
            // are drawn over the cell if present. Otherwise, we
            // draw the 'sprite' of the tile.
            if (tile_is_occupied(tile) && view->render_entities)
            {
                int32_t entity_draw_count = 0;

                if (tile->monster != NULL)
                {
                    entity_draw_count += 1;

                    TCOD_console_set_char_foreground(0, draw_x, draw_y, tile->monster->type_info->fore_color);    
                    TCOD_console_set_char(0, draw_x, draw_y, tile->monster->type_info->symbol);
                }

                if (tile->player != NULL)
                {
                    entity_draw_count += 1;

                    TCOD_console_set_char_foreground(0, draw_x, draw_y, TCOD_dark_cyan);
                    TCOD_console_set_char(0, draw_x, draw_y, '@');
                }

                if (entity_draw_count > 1)
                {
                    printf("Warning: More than one living entity present on '%s' tile at (%i, %i)\n", tile->type_info->name, x, y);
                }
            }
            else
            {
                switch (tile->type_info->type)
                {
                    // @Incomplete: Insert floor tile checkerboard

                    // @Incomplete: Insert wall tile visual connectivity

                    default: // This means there is not a special render routine for this tile.
                    {
                        TCOD_console_set_char_foreground(0, draw_x, draw_y, tile->type_info->fore_color);
                        TCOD_console_set_char(0, draw_x, draw_y, tile->type_info->symbol);
                    
                        break;
                    }
                }
            }
        }
    }
}

void render_stats(Player *player, StatsView *view)
{
    // @Incomplete Render stats here.
}

void render_rect(TCOD_Console *console, int32_t x, int32_t y, int32_t width, int32_t height, char c, TCOD_color_t fore_color, TCOD_color_t back_color)
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

void render_swap_foreground_and_background(TCOD_Console *console, int32_t x, int32_t y)
{
    TCOD_color_t fore_color = TCOD_console_get_char_foreground(console, x, y);
    TCOD_color_t back_color = TCOD_console_get_char_background(console, x, y);

    TCOD_console_set_char_background(console, x, y, fore_color, TCOD_BKGND_SET);
    TCOD_console_set_char_foreground(console, x, y, back_color);
}

void render_mask_xor(TCOD_Console *console, int32_t x, int32_t y, TCOD_color_t mask)
{
    TCOD_color_t fore_color = TCOD_console_get_char_foreground(console, x, y);
    TCOD_color_t back_color = TCOD_console_get_char_background(console, x, y);

    fore_color.r = fore_color.r ^ mask.r;
    fore_color.g = fore_color.g ^ mask.g;
    fore_color.b = fore_color.b ^ mask.b;

    back_color.r = back_color.r ^ mask.r;
    back_color.g = back_color.g ^ mask.g;
    back_color.b = back_color.b ^ mask.b;

    TCOD_console_set_char_background(console, x, y, back_color, TCOD_BKGND_SET);
    TCOD_console_set_char_foreground(console, x, y, fore_color);
}