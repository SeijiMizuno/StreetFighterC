#ifndef __MENU__
#define __MENU__

typedef struct {
    unsigned char option;
    unsigned char close;
} menu;

menu *menuCreate();
void runMenu(menu *menu, gameStatus *gameStatus);
void menuDestroy(menu *menu);

#endif