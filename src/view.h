#ifndef VIEW_H
#define VIEW_H 1

#include <stdint.h>
#include <libtcod.h>
#include "utilities.h"



typedef struct MapView
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;

    int32_t render_entities;

    TCOD_console_t console;

} MapView;

#endif