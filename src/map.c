#include "map.h"
#include "tile.h"
#include "player.h"
#include "monster.h"
#include "linked_list.h"
#include "view.h"
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

void screen_to_map(MapView *view, int32_t cx, int32_t cy, int32_t *out_tx, int32_t *out_ty)
{
    *out_tx = cx - view->x;
    *out_ty = cy - view->y;
}