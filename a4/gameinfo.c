#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "gameinfo.h"
#include "player.h"
#include "hitbox.h"

gameStatus *gameStatusCreate () {
    gameStatus *ret_gameStatus = (gameStatus*) malloc(sizeof(gameStatus));
    if (!ret_gameStatus)
        return NULL;

    // distancia inicial entre os players
    ret_gameStatus->screenPosition = 0;
    ret_gameStatus->playersDistance = INIT_X2_POS - (INIT_X1_POS + P_WIDTH);
    ret_gameStatus->playersIntersection = 0;

    return ret_gameStatus;
}

void updateGravity (player *element) {
    // atualiza posição y do player
    element->bodyHitbox->y += element->vy;
    element->vy += GRAVITY;

    if (element->bodyHitbox->y + element->bodyHitbox->height >= Y_GROUND) {
        element->bodyHitbox->y = Y_GROUND - element->bodyHitbox->height;
        element->vy = 0;
        element->onGround = 1;
    }
}

void updateLifeBar (player *p1, player *p2) {
    // fundo vermelho da lifebar
    al_draw_filled_rounded_rectangle(X_P1_LIFEBAR, Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,0,0));
    al_draw_filled_rounded_rectangle(X_P2_LIFEBAR, Y_LIFEBAR, X_P2_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,0,0));

    // vida amarela do jogador
    if (p1->isDamaged > 0) {
        p1->hp -= LIFE_DECAY_SPEED;
        if (p1->isDamaged - LIFE_DECAY_SPEED < 0)
            p1->isDamaged = 0;
        else 
            p1->isDamaged -= LIFE_DECAY_SPEED;
    }
    if (p1->hp >= 0){
        al_draw_filled_rounded_rectangle(X_P1_LIFEBAR + (LIFEBAR_W - p1->hp), Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,255,0));
    }
    
    if (p2->isDamaged > 0) {
        p2->hp -= LIFE_DECAY_SPEED;
        if (p2->isDamaged - LIFE_DECAY_SPEED < 0)
            p2->isDamaged = 0;
        else 
            p2->isDamaged -= LIFE_DECAY_SPEED;
    }
    if (p2->hp >= 0){
        al_draw_filled_rounded_rectangle(X_P2_LIFEBAR + (LIFEBAR_W - p2->hp), Y_LIFEBAR, X_P2_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,255,0));
    }
    // borda da lifebar
    al_draw_rounded_rectangle(X_P1_LIFEBAR, Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 3, 3, al_map_rgb(255,255,255), 2.5);
    al_draw_rounded_rectangle(X_P2_LIFEBAR, Y_LIFEBAR, X_P2_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 3, 3, al_map_rgb(255,255,255), 2.5);
}


unsigned char checkWallCollision (player *element) {
// void checkWallColision (player *p1, player *p2) {
    // checando p1
    if ((element->bodyHitbox->x + element->vx <= 0) || 
        ((element->bodyHitbox->x + element->bodyHitbox->width) + element->vx >= X_SCREEN))
        return 1;
    
    return 0;
}

unsigned char checkPlayerCollision (player *p1, player *p2) {

    // checando se os players estão se tocando na dimensão y
    if (((p1->bodyHitbox->y + p1->bodyHitbox->height > p2->bodyHitbox->y) &&
        (p1->bodyHitbox->y < p2->bodyHitbox->y + p2->bodyHitbox->height)) ||
        ((p2->bodyHitbox->y + p2->bodyHitbox->height > p1->bodyHitbox->y) &&
        (p2->bodyHitbox->y < p1->bodyHitbox->y + p1->bodyHitbox->height))) {

        // checando se os players estão se tocando na dimensão x
        if (p1->facing == 0) {  // player1 olhando para a direita (ao mesmo tempo que p2->facing == 1)
            if ((p1->bodyHitbox->x + p1->bodyHitbox->width + p1->vx) > (p2->bodyHitbox->x))
                return 1;
        }   
        else {
            if ((p2->bodyHitbox->x + p2->bodyHitbox->width - p1->vx) > (p1->bodyHitbox->x))
                return 1;
        }
    }

    return 0;
}

void updateFacing (player *p1, player *p2) {
    if (p1->facing == 0) {
        if (p1->bodyHitbox->x > p2->bodyHitbox->x + (p2->bodyHitbox->width / 2)) {
            toggleState(&p1->facing);
            toggleState(&p2->facing);
        }
    } else {
        if (p2->bodyHitbox->x > p1->bodyHitbox->x + (p1->bodyHitbox->width / 2)) {
            toggleState(&p1->facing);
            toggleState(&p2->facing);
        }
    }
}

void separatePlayers (player *p1, player *p2) {
    if (checkPlayerCollision(p1,p2)){
        if (!checkWallCollision(p1) && !checkWallCollision(p2)) {
            printf("AAAAAAAA\n");
            if ((p1->bodyHitbox->x + p1->bodyHitbox->width / 2) < (p2->bodyHitbox->x + p2->bodyHitbox->width / 2)) {
                p1->vx = -MOVE_STEP;
                p2->vx = MOVE_STEP;
            }
            else if ((p1->bodyHitbox->x + p1->bodyHitbox->width / 2) >= (p2->bodyHitbox->x + p2->bodyHitbox->width / 2)) {
                p1->vx = MOVE_STEP;
                p2->vx = -MOVE_STEP;
            }
        }
        else if (checkWallCollision(p1)) {
            p1->vx = 0;
            if (p1->facing == 0) 
                p2->vx = MOVE_STEP*2;
            else
                p2->vx = -MOVE_STEP*2;
        }
        else if (checkWallCollision(p2)) {
            p2->vx = 0;
            if (p2->facing == 0)
                p1->vx = MOVE_STEP*2;
            else
                p1->vx = -MOVE_STEP*2;
        }
        p1->bodyHitbox->x += p1->vx;
        p2->bodyHitbox->x += p2->vx;
    }
}

void updateJump (player *p1, player *p2) {
    // atualiza posição x do player no ar
    if (p1->airStun) {
        if (p1->airStun < 0) {
            if (!checkWallCollision(p1))
                p1->bodyHitbox->x -= MOVE_STEP;
            else
                p1->bodyHitbox->x = 0;
            p1->airStun++;
        }
        else {  // logicamente, airStun > 0
            if (!checkWallCollision(p1))
                p1->bodyHitbox->x += MOVE_STEP;
            else
                p1->bodyHitbox->x = X_SCREEN - p1->bodyHitbox->width;
            p1->airStun--;
        }
    }

    // atualiza posição x do player no ar
    if (p2->airStun) {
        if (p2->airStun < 0) {
            if (!checkWallCollision(p2))
                p2->bodyHitbox->x -= MOVE_STEP;
            else
                p2->bodyHitbox->x = 0;
            p2->airStun++;
        }
        else {  // logicamente, airStun > 0
            if (!checkWallCollision(p2))
                p2->bodyHitbox->x += MOVE_STEP;
            else
                p2->bodyHitbox->x = X_SCREEN - p2->bodyHitbox->width;
            p2->airStun--;
        }
    }
    // if (p1->facing == 0) {
    //     if ((p1->bodyHitbox->x + (p1->bodyHitbox->width / 2)) < 
    //         (p2->bodyHitbox->x + (p2->bodyHitbox->width / 2)))
    //         if (p1->airStun)
    // }
}

void gameStatusDestroy (gameStatus *gameStatus) {
    if (gameStatus)
        free (gameStatus);
} 