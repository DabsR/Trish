#ifndef TILE_H
#define TILE_H 1

#include <stdint.h>
#include <libtcod.h>

// Forward declarations
typedef struct Player  Player;
typedef struct Monster Monster;



typedef enum TileType
{
    TILE_AIR,
    TILE_FLOOR,
    TILE_WALL,

    __TILE_ENUM_LENGTH__

} TileType;



typedef struct TileTypeInfo
{
    TileType  type;
	char     *name;
	int32_t   is_visible;
	int32_t   is_solid;
	
	char symbol;
	TCOD_color_t fore_color;
	TCOD_color_t back_color;
} TileTypeInfo;



typedef struct Tile
{
    TileTypeInfo *type_info;
    Player       *player;
    Monster      *monster;

} Tile;



int32_t tile_is_occupied(Tile *tile);



void           tile_registry_initialise();
TileTypeInfo * tile_registry_lookup_type(TileType type);

#endif