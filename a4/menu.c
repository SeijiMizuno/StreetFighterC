#include <stdio.h>
#include <allegro5/allegro_ttf.h>

#include "gameinfo.h"
#include "menu.h"

const char *lines[3] = {
    "SinglePlayer",
    "MultiPlayer",
    "Quit"
};

menu *menuCreate() {
    menu *ret_menu = (menu*) malloc(sizeof(menu));
    if (!ret_menu)
        return NULL;
    
    ret_menu->option = 0;
    ret_menu->close = 0;

    return ret_menu;
}

void runMenu(menu *menu, gameStatus *gameStatus) {
    // if (menu->option)
    al_draw_text (gameStatus->font, al_map_rgb(255, 255, 255), 100, 100, 0, lines[0]);
    al_draw_text (gameStatus->font, al_map_rgb(255, 255, 255), 100, 200, 0, lines[1]);
    al_draw_text (gameStatus->font, al_map_rgb(255, 255, 255), 100, 300, 0, lines[2]);
}

void menuDestroy(menu *menu) {
    if (menu)
        free (menu);
}