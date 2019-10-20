#include "tile.h"
#include "utilities.h"



// Globals
int32_t      was_tile_registry_init = 0;
TileTypeInfo tile_registry[__TILE_ENUM_LENGTH__];



int32_t tile_is_occupied(Tile *tile)
{
    return (tile->monster != NULL || tile->player != NULL);
}



void tile_registry_initialise()
{
    // assert(!was_registry_init);
    if (was_tile_registry_init) return;
    
    was_tile_registry_init = 1;

    memset(tile_registry, 0, sizeof(tile_registry));

    tile_registry[TILE_AIR].type       = TILE_AIR;
    tile_registry[TILE_AIR].name       = "Air";
    tile_registry[TILE_AIR].symbol     = ' ';
    tile_registry[TILE_AIR].is_visible = 1;
    tile_registry[TILE_AIR].is_solid   = 1;
    tile_registry[TILE_AIR].back_color = TCOD_black;
    tile_registry[TILE_AIR].fore_color = TCOD_black;



    tile_registry[TILE_FLOOR].type       = TILE_FLOOR;
    tile_registry[TILE_FLOOR].name       = "Floor";
    tile_registry[TILE_FLOOR].symbol     = '.';
    tile_registry[TILE_FLOOR].is_visible = 1;
    tile_registry[TILE_FLOOR].is_solid   = 0;
    tile_registry[TILE_FLOOR].back_color = TCOD_black;
    tile_registry[TILE_FLOOR].fore_color = TCOD_gray;



    tile_registry[TILE_WALL].type       = TILE_WALL;
    tile_registry[TILE_WALL].name       = "Wall";
    tile_registry[TILE_WALL].symbol     = '#';
    tile_registry[TILE_WALL].is_visible = 1;
    tile_registry[TILE_WALL].is_solid   = 1;
    tile_registry[TILE_WALL].back_color = TCOD_black;
    tile_registry[TILE_WALL].fore_color = TCOD_white;
}

TileTypeInfo * tile_registry_lookup_type(TileType type)
{
    assert(was_tile_registry_init);
    assert(type >= TILE_AIR && type < __TILE_ENUM_LENGTH__);

    return &tile_registry[type];
}