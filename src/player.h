#ifndef PLAYER_H
#define PLAYER_H 1

#include <stdint.h>

// Forward declarations
typedef struct Tile Tile;
typedef struct Map Map; 



typedef struct Player
{
    int32_t x;
    int32_t y;

    Tile *tile;

} Player;

typedef struct MoveResult
{
    int32_t success;
    Tile   *current_tile;
    Tile   *target_tile;

} MoveResult;



Player *    player_create(Map *map, int32_t x, int32_t y);
MoveResult  player_move(Player *player, Map *map, int32_t dx, int32_t dy);
void        player_free(Player *player);

#endif





