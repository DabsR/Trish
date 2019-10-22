#include <stdio.h>
#include <stdlib.h>
#include <libtcod.h>

#include "tile.h"
#include "map.h"
#include "player.h"
#include "monster.h"
#include "utilities.h"
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

MapView   mapview;


typedef enum EditorState
{
    EDITORSTATE_PICK,
    EDITORSTATE_PLACE_TILE,
    EDITORSTATE_PLACE_MONSTER,
    EDITORSTATE_PAUSE

} EditorState;

EditorState editorstate             = EDITORSTATE_PLACE_TILE;
EditorState editorstate_before_pick = EDITORSTATE_PLACE_TILE;
Tile       *selected_tile           = NULL;
int32_t     select_x                = 0;
int32_t     select_y                = 0;
TileType    editor_tiletype         = TILE_AIR;
MonsterType editor_monstertype      = MONSTER_SLIME;



void simulate_game()
{
    switch (gamestate)
    {
        case GAMESTATE_MOVE:
        {
            TCOD_console_clear(0);
            render_map(map, &mapview);
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
        mapview.render_entities = !mapview.render_entities;
    }

    TCOD_console_clear(0);

    switch (editorstate)
    {
        case EDITORSTATE_PLACE_TILE:
        case EDITORSTATE_PLACE_MONSTER:
        {
            screen_to_map(&mapview, mouse.cx, mouse.cy, &select_x, &select_y);
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
                        selected_tile->type_info = tile_registry_lookup_type(editor_tiletype);
                        
                        // @Incomplete: Maybe we want ghost-like enemies?
                        // Remove the monster if the tile is solid because that is impossible.
                        if (selected_tile->type_info->is_solid && selected_tile->monster)
                        {
                            monster_free(selected_tile->monster, map);
                        }
                    }
                    else if (editorstate == EDITORSTATE_PLACE_MONSTER)
                    {
                        // If we are placing a monster in a solid tile, make it
                        // compatible with that tile.
                        if (selected_tile->type_info->is_solid)
                        {
                            selected_tile->type_info = tile_registry_lookup_type(TILE_FLOOR);
                        }

                        if (selected_tile->monster)
                        {
                            selected_tile->monster->type_info = monster_registry_lookup_type(editor_monstertype);
                            selected_tile->monster->health    = selected_tile->monster->type_info->health;
                        }
                        else
                        {
                            monster_create(editor_monstertype, map, select_x, select_y);
                        }
                    }
                }
            }

            render_map(map, &mapview);
            
            if (selected_tile)
            {
                render_mask_xor(0, mouse.cx, mouse.cy, TCOD_white);
            }

            if (mapview.render_entities)
            {
                // @Incomplete: Still using the view macros here! Migrate to view structures.
                TCOD_console_printf(0, STATSVIEW_X, STATSVIEW_Y, "Entities (Show)");
            }
            else
            {
                // @Incomplete: Still using the view macros here! Migrate to view structures.
                TCOD_console_printf(0, STATSVIEW_X, STATSVIEW_Y, "Entities (Hide)");
            }

            if (editorstate == EDITORSTATE_PLACE_TILE)
            {
                // @Incomplete: Still using the view macros here! Migrate to view structures.
                TileTypeInfo *type_info = tile_registry_lookup_type(editor_tiletype);
                TCOD_console_printf(0, STATSVIEW_X, STATSVIEW_Y + 1, "Placing tile %s", type_info->name);
            }
            else if (editorstate == EDITORSTATE_PLACE_MONSTER)
            {
                MonsterTypeInfo *type_info = monster_registry_lookup_type(editor_monstertype);
                // @Incomplete: Still using the view macros here! Migrate to view structures.
                TCOD_console_printf(0, STATSVIEW_X, STATSVIEW_Y + 1, "Placing monster %s", type_info->name);
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

                tile_registry_initialise();
                monster_registry_initialise();

                map     = map_create(40, 22);
                player  = player_create(map, 3, 5);
                
                // @Incomplete: I want to remove these macros
                // eventually. They were integrated here with the
                // view for compatibility.
                mapview.x       = MAPVIEW_X;
                mapview.y       = MAPVIEW_Y;
                mapview.width   = MAPVIEW_WIDTH;
                mapview.height  = MAPVIEW_HEIGHT;    
                mapview.render_entities = 1;            
                mapview.console = 0;

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

                break;
            }



            default: break;
        }

    } while (appstate != APPSTATE_QUIT);

    TCOD_quit();

    return 0;
}
