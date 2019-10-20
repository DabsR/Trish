#include "monster.h"
#include "tile.h"
#include "map.h"
#include "linked_list.h"
#include "utilities.h"



// Globals
int32_t         was_monster_registry_init = 0;
MonsterTypeInfo monster_registry[__MONSTER_ENUM_LENGTH__];



void monster_registry_initialise()
{
    if (was_monster_registry_init) return;

    was_monster_registry_init = 1;

    memset(monster_registry, 0, sizeof(monster_registry));

    monster_registry[MONSTER_SLIME].name       = "Slime";
    monster_registry[MONSTER_SLIME].symbol     = 's';
    monster_registry[MONSTER_SLIME].health     = 3;
    monster_registry[MONSTER_SLIME].damage_min = 0;
    monster_registry[MONSTER_SLIME].damage_max = 2;
    monster_registry[MONSTER_SLIME].accuracy   = 30;
    monster_registry[MONSTER_SLIME].speed      = 1;
    monster_registry[MONSTER_SLIME].fore_color = TCOD_green;
}

MonsterTypeInfo * monster_registry_lookup_type(MonsterType type)
{
    assert(was_monster_registry_init);
    assert(type >= 0 && type < __MONSTER_ENUM_LENGTH__);

    return &monster_registry[type];
}


Monster * monster_create(MonsterType type, Map *map, int32_t x, int32_t y)
{
    assert(map);

    Tile *spawn_tile = map_get_tile(map, x, y);
    assert(spawn_tile && spawn_tile->monster == NULL);

    MonsterTypeInfo *type_info = monster_registry_lookup_type(type);
    Monster         *monster   = allocate(Monster);

    monster->type_info     = type_info;
    monster->health        = type_info->health;
    monster->x             = x;
    monster->y             = y;
    monster->tile          = spawn_tile;
    monster->tile->monster = monster;
    linkedlist_push(map->monsters, monster);

    return monster;
}

void monster_free(Monster *monster)
{
    assert(monster && monster->tile->monster == monster);

    monster->tile->monster = NULL;
    free(monster);
}