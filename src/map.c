#include "map.h"
#include "tile.h"
#include "player.h"
#include "monster.h"
#include "linked_list.h"
#include "utilities.h"
#include "global_settings.h"



Map * map_create(int32_t width, int32_t height)
{
    Map *map      = allocate(Map);
    map->width    = width;
    map->height   = height;
    map->tiles    = array(Tile, width * height);
    map->monsters = linkedlist_create();

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            initialise(&map->tiles[x + y * width]);

            if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
            {
                map->tiles[x + y * width].type_info = tile_registry_lookup_type(TILE_WALL);
            }
            else
            {
                map->tiles[x + y * width].type_info = tile_registry_lookup_type(TILE_FLOOR);   
            }
        }
    }

    return map;
}

void map_free(Map *map)
{
    player_free(map->player);

    LinkedNode *node = map->monsters->head;
    while (node)
    {
        Monster* monster = (Monster*) node->data;
        monster_free(monster); 

        node = node->next;
    }

    linkedlist_free(map->monsters);

    free(map->tiles);
    free(map);
}

void map_draw(Map *map)
{
    // @Incomplete: We need to account
    // for map scrolling.

    for (size_t y = 0; y < map->height; y++)
    {
        for (size_t x = 0; x < map->width; x++)
        {
            Tile *tile     = &map->tiles[x + y * map->width];
            int32_t draw_x = MAPVIEW_X + x;
            int32_t draw_y = MAPVIEW_Y + y;

            if (!tile->type_info->is_visible) continue;

            // The background of the tile is drawn regardless
            // of what is present on the tile.
            TCOD_console_set_char_background(0, draw_x, draw_y, tile->type_info->back_color, 0);
                
            if (tile->player == NULL && tile->monster == NULL)
            {
                TCOD_console_set_char_foreground(0, draw_x, draw_y, tile->type_info->fore_color);
                TCOD_console_set_char(0, draw_x, draw_y, tile->type_info->symbol);
            }
            else if (tile->monster != NULL)
            {
                TCOD_console_set_char_foreground(0, draw_x, draw_y, tile->monster->type_info->fore_color);    
                TCOD_console_set_char(0, draw_x, draw_y, tile->monster->type_info->symbol);
            }
            else if (tile->player != NULL)
            {
                TCOD_console_set_char_foreground(0, draw_x, draw_y, TCOD_dark_cyan);
                TCOD_console_set_char(0, draw_x, draw_y, '@');
            }
        }
    }
}

Tile * map_get_tile(Map *map, int32_t x, int32_t y)
{
    // @Cleanup
    // What if we are moving against the edge of the map somehow?
    // Maybe assertion here is not the best idea.
    // It could be argued we shoul have a permanent border of one
    // tile on the edges.
    //
    // assert(x >= 0 && x < map->width && y >= 0 && y < map->height);

    int32_t in_bounds = (x >= 0 && x < map->width && y >= 0 && y < map->height);

    if (in_bounds)
    {
        return &map->tiles[x + y * map->width];
    }
    else
    {
        return NULL;
    }
}