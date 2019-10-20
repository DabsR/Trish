#ifndef MONSTER_H
#define MONSTER_H 1

#include <stdint.h>
#include <libtcod.h>



// Fordward declarations
typedef struct Tile Tile;
typedef struct Map Map;



typedef enum MonsterType
{
    MONSTER_SLIME,

    __MONSTER_ENUM_LENGTH__

} MonsterType;

typedef struct MonsterTypeInfo
{
    char *name;
    char  symbol;

    int32_t health;
    int32_t damage_min;
    int32_t damage_max;
    int32_t accuracy;   // A value between 0 and 100
    int32_t speed;      // The number of tiles this moves in one tick.

    TCOD_color_t fore_color;

} MonsterTypeInfo;

typedef struct Monster
{
    MonsterTypeInfo *type_info;

    int32_t x;
    int32_t y;
    int32_t health;

    Tile *tile;

} Monster;



Monster * monster_create(MonsterType type, Map *map, int32_t x, int32_t y);
void      monster_free(Monster *monster);

void              monster_registry_initialise();
MonsterTypeInfo * monster_registry_lookup_type(MonsterType type);

#endif