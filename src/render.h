#ifndef RENDER_H
#define RENDER_H 1

#include <stdint.h>
#include <libtcod.h>



// Forward declarations
typedef struct Player Player;
typedef struct Map Map;
typedef struct EventView EventView;
typedef struct MapView MapView;
typedef struct StatsView StatsView;
typedef struct TCOD_ColorRGB TCOD_color_t;
typedef struct TCOD_Console TCOD_Console;


void render_events(EventView *view);
void render_map(Map *map, MapView *view);
void render_stats(Player *player, StatsView *view);
void render_rect(TCOD_Console *console, int32_t x, int32_t y, int32_t width, int32_t height, char c, TCOD_color_t fore_color, TCOD_color_t back_color);
void render_swap_foreground_and_background(TCOD_Console *console, int32_t x, int32_t y);
void render_mask_xor(TCOD_Console *console, int32_t x, int32_t y, TCOD_color_t mask);

#endif