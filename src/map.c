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
    map->is_being_freed = 1;

    player_free(map->player);

    LinkedNode *node = map->monsters->head;
    while (node)
    {
        Monster* monster = (Monster*) node->data;
        monster_free(monster, map); 

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

NeighbourInfo map_get_tile_neighbours(Map *map, int32_t x, int32_t y)
{
    NeighbourInfo neighbour_info;
    
    neighbour_info.origin = map_get_tile(map, x, y);
    neighbour_info.north  = map_get_tile(map, x, y - 1);
    neighbour_info.east   = map_get_tile(map, x + 1, y);
    neighbour_info.south  = map_get_tile(map, x, y + 1);
    neighbour_info.west   = map_get_tile(map, x - 1, y);
    neighbour_info.count  = 0;

    if (neighbour_info.north) neighbour_info.count += 1;
    if (neighbour_info.east)  neighbour_info.count += 1;
    if (neighbour_info.south) neighbour_info.count += 1;
    if (neighbour_info.west)  neighbour_info.count += 1;

    return neighbour_info;
}

NeighbourInfo map_get_tile_neighbours_of_type(Map *map, int32_t x, int32_t y, TileType type)
{
    NeighbourInfo neighbour_info;
    
    neighbour_info.origin = map_get_tile(map, x, y);
    neighbour_info.north  = map_get_tile(map, x, y - 1);
    neighbour_info.east   = map_get_tile(map, x + 1, y);
    neighbour_info.south  = map_get_tile(map, x, y + 1);
    neighbour_info.west   = map_get_tile(map, x - 1, y);
    neighbour_info.count  = 0;

    if (!(neighbour_info.north && neighbour_info.north->type_info->type == type)) neighbour_info.north = NULL;
    if (!(neighbour_info.east  && neighbour_info.east->type_info->type  == type)) neighbour_info.east  = NULL;
    if (!(neighbour_info.south && neighbour_info.south->type_info->type == type)) neighbour_info.south = NULL;
    if (!(neighbour_info.west  && neighbour_info.west->type_info->type  == type)) neighbour_info.west  = NULL;

    if (neighbour_info.north) neighbour_info.count += 1;
    if (neighbour_info.east)  neighbour_info.count += 1;
    if (neighbour_info.south) neighbour_info.count += 1;
    if (neighbour_info.west)  neighbour_info.count += 1;

    return neighbour_info;
}

void screen_to_map(MapView *view, int32_t cx, int32_t cy, int32_t *out_tx, int32_t *out_ty)
{
    *out_tx = cx - view->x;
    *out_ty = cy - view->y;
}