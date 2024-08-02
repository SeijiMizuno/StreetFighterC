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
#define INIT_X2_POS X_SCREEN - P_WIDTH - 100

#define GRAVITY 5
// #define GRAVITY 1

typedef struct {
    signed short screenPosition;
    unsigned short playersDistance;
    unsigned short playersIntersection; // representa o tamanho no eixo X da intersecção entre os player
} gameStatus;

gameStatus *gameStatusCreate ();
void updateGravity (player *element);
void updateLifeBar (player *p1, player *p2);
void updateAtkCooldown (player *element);
void updateComboCooldown (player *element);
unsigned char checkAtkHit (player *p1, player *p2);
unsigned char checkComboHit (player *p1, player *p2);
unsigned char checkWallCollision (player *element);
unsigned char checkPlayerCollision (player *p1, player *p2);
void updateFacing (player *p1, player *p2);
void separatePlayers (player *p1, player *p2);
void updateComboHitbox (player *p1, player *p2);
void updateAtkHitbox (player *p1, player *p2);
void updateJump (player *p1, player *p2);
void gameStatusDestroy (gameStatus *gameStatus);

#endif