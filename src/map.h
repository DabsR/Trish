#ifndef MAP_H
#define MAP_H 1

#include <stdint.h>



// Forward declarations
typedef enum TileType TileType;
typedef struct Tile Tile;
typedef struct Player Player;
typedef struct Monster Monster;
typedef struct LinkedList LinkedList;
typedef struct MapView MapView;
typedef struct TCOD_Console TCOD_Console;



typedef struct Map
{
    int32_t width;
    int32_t height;

    Tile   *tiles;

    Player     *player;
    LinkedList *monsters;

    int32_t is_being_freed;

} Map;



typedef struct NeighbourInfo
{
    Tile *origin;

    Tile *north;
    Tile *east;
    Tile *south;
    Tile *west;

    int32_t count;

} NeighbourInfo;



Map *  map_create(int32_t width, int32_t height);
void   map_free(Map *map);
Tile * map_get_tile(Map *map, int32_t x, int32_t y);

NeighbourInfo map_get_tile_neighbours(Map *map, int32_t x, int32_t y);
NeighbourInfo map_get_tile_neighbours_of_type(Map *map, int32_t x, int32_t y, TileType type);

void screen_to_map(MapView *view, int32_t cx, int32_t cy, int32_t *out_tx, int32_t *out_ty);  

#endif