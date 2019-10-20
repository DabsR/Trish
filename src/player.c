#include "player.h"
#include "tile.h"
#include "map.h"
#include "utilities.h"


Player * player_create(Map *map, int32_t x, int32_t y)
{
    assert(map && !map->player);

    Player *player = allocate(Player);

    Tile *spawn_tile = map_get_tile(map, x, y);
    assert(spawn_tile);

    player->x = x;
    player->y = y;
    player->tile = spawn_tile;
    
    map->player  = player;

    spawn_tile->player = player;
    
    return player;
}

MoveResult player_move(Player *player, Map *map, int32_t dx, int32_t dy)
{
    assert(player && map);

    Tile *current_tile = map_get_tile(map, player->x, player->y);
    Tile *target_tile  = map_get_tile(map, player->x + dx, player->y + dy);

    MoveResult moveResult;
    moveResult.success      = 0;
    moveResult.current_tile = current_tile;
    moveResult.target_tile  = target_tile;

    if (target_tile && (current_tile != target_tile))
    {
        if (target_tile->type_info->is_solid == 0)
        {
            if (tile_is_occupied(target_tile))
            {
                // @Incomplete: We need to handle tiles
                // that are occupied by monsters.
            }
            else
            {
                target_tile->player  = player;
                player->tile         = target_tile;
                current_tile->player = NULL;

                player->x += dx;
                player->y += dy;

                moveResult.success      = 1;
                moveResult.current_tile = target_tile;
            }
        }
    }

    return moveResult;
}

void player_free(Player *player)
{
    assert(player && player->tile->player == player);

    player->tile->player = NULL;
    free(player);
}