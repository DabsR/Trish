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

EditorState editorstate        = EDITORSTATE_PLACE_TILE;
Tile       *selected_tile      = NULL;
int32_t     select_x           = 0;
int32_t     select_y           = 0;
TileType    editor_tiletype    = TILE_AIR;
MonsterType editor_monstertype = MONSTER_SLIME;



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
    screen_to_map(mouse.cx, mouse.cy, &select_x, &select_y);
    selected_tile = map_get_tile(map, select_x, select_y);

    TCOD_key_t key;
    TCOD_sys_check_for_event(TCOD_EVENT_KEY_PRESS, &key, NULL);

    
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
