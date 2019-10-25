#include <stdio.h>
#include <stdlib.h>
#include <libtcod.h>

#include "tile.h"
#include "map.h"
#include "player.h"
#include "monster.h"
#include "utilities.h"
#include "event_log.h"
#include "view.h"
#include "render.h"

#include "global_settings.h"



typedef enum GameMode
{
    GAMEMODE_SINGLEPLAYER,
    GAMEMODE_EDITOR

} GameMode;

GameMode gamemode = GAMEMODE_EDITOR;


typedef enum AppState
{
    APPSTATE_INIT,
    APPSTATE_RUNNING,
    APPSTATE_QUIT

} AppState;

AppState appstate = APPSTATE_INIT;


typedef enum GameState
{
    GAMESTATE_MOVE,
    GAMESTATE_PAUSE

} GameState;

GameState gamestate = GAMESTATE_MOVE;
Map      *map       = NULL;
Player   *player    = NULL;

EventView event_view;
MapView   map_view;
StatsView stats_view;


typedef enum EditorState
{
    EDITORSTATE_PICK,
    EDITORSTATE_PLACE_TILE,
    EDITORSTATE_PLACE_MONSTER,
    EDITORSTATE_PLACE_PLAYER,
    EDITORSTATE_PAUSE

} EditorState;

EditorState editorstate             = EDITORSTATE_PLACE_TILE;
EditorState editorstate_before_pick = EDITORSTATE_PLACE_TILE;
Tile       *selected_tile           = NULL;
int32_t     select_x                = 0;
int32_t     select_y                = 0;
TileType    editor_tiletype         = TILE_AIR;
MonsterType editor_monstertype      = MONSTER_SLIME;



void place_player(int32_t x, int32_t y)
{
    if (player == NULL)
    {
        player = player_create(map, x, y);
    }
    else
    {
        Tile *tile = map_get_tile(map, x, y);
        if (tile)
        {
            player->tile->player = NULL;
            player->tile = tile;
            player->tile->player = player;
            player->x = x;
            player->y = y;
        }

        if (tile->type_info->is_solid)
        {
            TileTypeInfo *floor_type_info = tile_registry_lookup_type(TILE_FLOOR);
            tile->type_info = floor_type_info;
        }

        if (tile->monster)
        {
            monster_free(tile->monster, map);
        }
    }
}

void place_monster(MonsterType type, int32_t x, int32_t y)
{
    Tile *tile = map_get_tile(map, x, y);
    if (tile)
    {
        Monster *monster = tile->monster;

        if (monster == NULL)
        {
            monster_create(type, map, x, y);
        }
        else
        {
            MonsterTypeInfo *type_info = monster_registry_lookup_type(type);
            monster->type_info = type_info;
            monster->health    = type_info->health;
        }

        if (tile->type_info->is_solid)
        {
            TileTypeInfo *floor_type_info = tile_registry_lookup_type(TILE_FLOOR);
            tile->type_info = floor_type_info;
        }

        if (tile->player)
        {
            player_free(player, map);
            player = NULL;
        }
    }
}

void place_tile(TileType type, int32_t x, int32_t y)
{
    Tile *tile = map_get_tile(map, x, y);
    if (tile)
    {
        TileTypeInfo *type_info = tile_registry_lookup_type(type);
        tile->type_info = type_info;
        
        if (type_info->is_solid && tile->player)
        {
            player_free(player, map);
            player = NULL;
        }
        else if (type_info->is_solid && tile->monster)
        {
            monster_free(tile->monster, map);
        }
    }
}

void simulate_game()
{
    switch (gamestate)
    {
        case GAMESTATE_MOVE:
        {
            TCOD_console_clear(0);
            render_map(map, &map_view);
            TCOD_console_flush();

            TCOD_key_t key;
            TCOD_sys_check_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL);

            int32_t dx = 0;
            int32_t dy = 0;

            switch (key.vk)
            {
                case TCODK_UP:    dy = -1; break;
                case TCODK_DOWN:  dy =  1; break;
                case TCODK_RIGHT: dx =  1; break;
                case TCODK_LEFT:  dx = -1; break;
                case TCODK_ESCAPE: gamestate = GAMESTATE_PAUSE;
                default: break;
            }

            if (dx != 0 || dy != 0)
            {
                MoveResult moveResult = player_move(player, map, dx, dy);
                if (!moveResult.success)
                {
                    // @Incomplete: This is where we need to
                    // print to the event log.
                }
            }

            break;
        }
    }
}

void simulate_editor()
{
    TCOD_mouse_t mouse = TCOD_mouse_get_status();

    TCOD_key_t key;
    TCOD_sys_check_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL);

    if (key.vk == TCODK_TAB)
    {
        if (editorstate == EDITORSTATE_PICK)
        {
            editorstate = editorstate_before_pick;
        }
        else
        {
            editorstate_before_pick = editorstate;
            editorstate = EDITORSTATE_PICK;
        }
    }

    if (key.vk == TCODK_1)
    {
        map_view.render_entities = !map_view.render_entities;
    }

    TCOD_console_clear(0);

    switch (editorstate)
    {
        case EDITORSTATE_PLACE_TILE:
        case EDITORSTATE_PLACE_MONSTER:
        case EDITORSTATE_PLACE_PLAYER:
        {
            screen_to_map(&map_view, mouse.cx, mouse.cy, &select_x, &select_y);
            selected_tile = map_get_tile(map, select_x, select_y);

            if (mouse.lbutton)
            {
                // @Incomplete: It would be better to have two selected tile types:
                // One for the left mouse button and one for the right mouse button
                // just like most image editing software. Or perhaps we should make
                // that copy tiles like in RPG maker. Not important right now.
                //
                // Jade (22 October 2019)
                
                if (selected_tile)
                {
                    if (editorstate == EDITORSTATE_PLACE_TILE)
                    {
                        place_tile(editor_tiletype, select_x, select_y);
                    }
                    else if (editorstate == EDITORSTATE_PLACE_MONSTER)
                    {
                        place_monster(editor_monstertype, select_x, select_y);
                    }
                    else if (editorstate == EDITORSTATE_PLACE_PLAYER)
                    {
                        place_player(select_x, select_y);
                    }
                }
            }

            render_events(&event_view);
            render_map(map, &map_view);
            
            if (selected_tile)
            {
                render_mask_xor(0, mouse.cx, mouse.cy, TCOD_white);
            }

            if (map_view.render_entities)
            {
                TCOD_console_printf(0, stats_view.x, stats_view.y, "Entities (Show)");
            }
            else
            {
                TCOD_console_printf(0, stats_view.x, stats_view.y, "Entities (Hide)");
            }

            if (editorstate == EDITORSTATE_PLACE_TILE)
            {
                TileTypeInfo *type_info = tile_registry_lookup_type(editor_tiletype);
                TCOD_console_printf(0, stats_view.x, stats_view.y + 1, "Placing tile %s", type_info->name);
            }
            else if (editorstate == EDITORSTATE_PLACE_MONSTER)
            {
                MonsterTypeInfo *type_info = monster_registry_lookup_type(editor_monstertype);
                TCOD_console_printf(0, stats_view.x, stats_view.y + 1, "Placing monster %s", type_info->name);
            }
            else if (editorstate == EDITORSTATE_PLACE_PLAYER)
            {
                TCOD_console_printf(0, stats_view.x, stats_view.y + 1, "Placing player");
            }
            

            break;
        }






        case EDITORSTATE_PICK:
        {
            for (size_t i = 0; i < __TILE_ENUM_LENGTH__; i++)
            {
                TileType      type      = (TileType) i;
                TileTypeInfo *type_info = tile_registry_lookup_type(type);

                char   option_text[20];
                size_t option_text_length;

                sprintf(option_text, "%li %s", i, type_info->name);
                option_text_length = strlen(option_text);

                TCOD_console_print(0, 0, i, option_text);

                // Check if the mouse is over the option and highlight it.
                if (mouse.cx >= 0 && mouse.cx < option_text_length && mouse.cy == i)
                {
                    for (size_t j = 0; j < option_text_length; j++)
                    {
                        render_swap_foreground_and_background(0, j, i);
                    }

                    if (mouse.lbutton_pressed)
                    {
                        editor_tiletype = type;
                        editorstate = EDITORSTATE_PLACE_TILE;
                        break;
                    }
                }
            }

            for (size_t i = 0; i < __MONSTER_ENUM_LENGTH__; i++)
            {
                MonsterType      type      = (MonsterType) i;
                MonsterTypeInfo *type_info = monster_registry_lookup_type(type);

                char   option_text[20];
                size_t option_text_length;

                sprintf(option_text, "%li %s", i, type_info->name);
                option_text_length = strlen(option_text);

                TCOD_console_print(0, 22, i, option_text);

                // Check if the mouse is over the option and highlight it.
                if (mouse.cx >= 22 && mouse.cx < 22 + option_text_length && mouse.cy == i)
                {
                    for (size_t j = 0; j < option_text_length; j++)
                    {
                        render_swap_foreground_and_background(0, 22 + j, i);
                    }

                    if (mouse.lbutton_pressed)
                    {
                        editor_monstertype = type;
                        editorstate = EDITORSTATE_PLACE_MONSTER;
                    }
                }
            }

            TCOD_console_print(0, 0, WINDOW_HEIGHT - 1, "@ Player");

            // Check if the mouse is over the option and highlight it.
            if (mouse.cx >= 0 && mouse.cx < 8 && mouse.cy == WINDOW_HEIGHT - 1)
            {
                for (size_t j = 0; j < 8; j++)
                {
                    render_swap_foreground_and_background(0, j, WINDOW_HEIGHT - 1);
                }

                if (mouse.lbutton_pressed)
                {
                    editorstate = EDITORSTATE_PLACE_PLAYER;
                }
            }

            break;
        }


        
        default: break;
    }

    TCOD_console_flush();
}

int main(int argc, char **argv)
{
    do
    {
        if (TCOD_console_is_window_closed())
        {
            appstate = APPSTATE_QUIT;
        }



        switch (appstate)
        {
            case APPSTATE_INIT:
            {
                TCOD_console_init_root(WINDOW_WIDTH, WINDOW_HEIGHT, "Trish", false, TCOD_RENDERER_SDL2);
                TCOD_console_set_custom_font("terminal.png", 0, 16, 16);

                tile_registry_initialise();
                monster_registry_initialise();

                map     = map_create(40, 22);
                player  = player_create(map, 3, 5);



                eventlog_initialise(32);


                
                event_view.x       = 0;
                event_view.y       = 0;
                event_view.width   = 40;
                event_view.height  = 3;
                event_view.console = 0;

                map_view.x               = 0;
                map_view.y               = 4;
                map_view.width           = 40;
                map_view.height          = 22;
                map_view.render_entities = 1;            
                map_view.console         = 0;

                stats_view.x       = 0;
                stats_view.y       = 27;
                stats_view.width   = 40;
                stats_view.height  = 3;
                stats_view.console = 0;



                monster_create(MONSTER_SLIME, map, 1, 1);
                monster_create(MONSTER_SLIME, map, 7, 3);
                
                appstate = APPSTATE_RUNNING;

                break;
            }



            case APPSTATE_RUNNING:
            {
                switch (gamemode)
                {
                    case GAMEMODE_SINGLEPLAYER: simulate_game();   break;
                    case GAMEMODE_EDITOR:       simulate_editor(); break;
                    default: break;
                }

                break;
            }



            case APPSTATE_QUIT:
            {
                map_free(map);
                eventlog_terminate();

                break;
            }



            default: break;
        }

    } while (appstate != APPSTATE_QUIT);

    TCOD_quit();

    return 0;
}
