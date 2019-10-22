#ifndef MAP_H
#define MAP_H 1

#include <stdint.h>



// Forward declarations
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


} Map;



Map *  map_create(int32_t width, int32_t height);
void   map_free(Map *map);
Tile * map_get_tile(Map *map, int32_t x, int32_t y);

void   screen_to_map(MapView *view, int32_t cx, int32_t cy, int32_t *out_tx, int32_t *out_ty);  

#endif