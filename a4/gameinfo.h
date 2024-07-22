#ifndef __GAMEINFO__
#define __GAMEINFO__

#include "player.h"

#define X_SCREEN 800
#define Y_SCREEN 600

#define Y_GROUND 550

#define Y_LIFEBAR 40
#define X_P1_LIFEBAR 50
#define X_P2_LIFEBAR 425
#define LIFEBAR_W 325
#define LIFEBAR_H 30

#define LIFE_DECAY_SPEED 10

#define INIT_X1_POS 100
#define INIT_X2_POS 500

#define GRAVITY 4

typedef struct {
    unsigned short playersDistance;
    signed short screenPosition;
} gameStatus;

gameStatus *gameStatusCreate ();
void updateGravity (player *element);
void updateLifeBar (player *p1, player *p2);
unsigned char checkWallCollision (player *element);
unsigned char checkPlayerCollision (player *p1, player *p2);
void updateFacing (player *p1, player *p2);
void gameStatusDestroy (gameStatus *gameStatus);

#endif