#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "hitbox.h"
#include "joystick.h"
#include "gameinfo.h"

player *playerCreate (unsigned short init_x, unsigned short init_y, unsigned short widht, unsigned short height) {
    player *ret_player = (player*) malloc(sizeof(player));
    if (!ret_player)
        return NULL;
    
    ret_player->character_id = 0;
    ret_player->hp = LIFEBAR_W;
    ret_player->vx = 0;
    ret_player->vy = 0;
    ret_player->onGround = 1;
    ret_player->isCrouch = 0;
    ret_player->airStun = 0;
    ret_player->getUp = 0;
    ret_player->roundWin = 0;
    ret_player->isDamaged = 0;
    ret_player->atkCooldown = 0;
    ret_player->isHuman = 1;
    ret_player->facing = 0;
    ret_player->sprite = NULL;
    ret_player->action = joystickCreate();
    ret_player->bodyHitbox = hitboxCreate(init_x, init_y, widht, height);
    ret_player->atkHitbox = hitboxCreate(0,0,0,0);

    return ret_player;
}

void toggleState (unsigned char *toBeToggled) {
    *toBeToggled = !(*toBeToggled);
}

void setAtkCooldown (player *player) {
    if (player->atkCooldown == 0) 
        player->atkCooldown = ATK_COOLDOWN;
}

void updateAtkCooldown (player *player) {
    if (player->atkCooldown - 1 >= 0)
        player->atkCooldown--;
    else
        player->atkCooldown = 0;
}

void playerAction (player *p1, player *p2, unsigned char action) {
    switch (action) {
        // // // //
        // player 1
        case 0: // cima (ativado apenas se o player estiver onGround)
            // tratamento de movimentos laterais
            if (p1->onGround) {
                p1->onGround = 0;
                p1->vy = -JUMP_VEL;
            }
            break;
            
        case 1: // esquerda
            p1->vx = -MOVE_STEP;
            if (checkWallCollision(p1))
                p1->bodyHitbox->x = 0;
            else if (checkPlayerCollision(p1, p2)) {
                if (checkWallCollision(p2)) {
                    p2->bodyHitbox->x = 0;
                    p1->bodyHitbox->x = (p1->bodyHitbox->width) + 5;
                }
                else {
                    p2->vx = p1->vx + 7;
                    p2->bodyHitbox->x += p2->vx;
                    p1->bodyHitbox->x += p1->vx + 7;
                }
            }
            else {
                p1->bodyHitbox->x += p1->vx;
            }
            
            p1->vx = 0;
            break;
            
        case 2: // baixo
            if (p1->onGround)
                if ((p1->isCrouch == 1)){
                    p1->bodyHitbox->height = P_CROUCH_HEIGHT;
                    p1->bodyHitbox->y = Y_GROUND - p1->bodyHitbox->height;
                }
                else {
                    p1->bodyHitbox->height = P_HEIGHT;
                    p1->bodyHitbox->y = Y_GROUND - p1->bodyHitbox->height;
                    p1->getUp = 0;
                }
            break;
            
        case 3: // direita
            p1->vx = MOVE_STEP;
            if (checkWallCollision(p1))
                p1->bodyHitbox->x = X_SCREEN - p1->bodyHitbox->width;
            else if (checkPlayerCollision(p1, p2)) {
                if (checkWallCollision(p2)) {
                    p2->bodyHitbox->x = X_SCREEN - p2->bodyHitbox->width;
                    p1->bodyHitbox->x = X_SCREEN - (p1->bodyHitbox->width * 2) - 5;
                }
                else {
                    p2->vx = p1->vx - 7;
                    p2->bodyHitbox->x += p2->vx;
                    p1->bodyHitbox->x += p1->vx - 7;
                }
            }
            else {
                p1->bodyHitbox->x += p1->vx;
            }
            p1->vx = 0;
            p2->vx = 0;
            break;
            
        case 4: // light punch
            p1->isDamaged += L_PUNCH_DAM;
            break;

        case 5: // hard punch
            p1->isDamaged += H_PUNCH_DAM;
            break;

        case 6: // light kick
            p1->isDamaged += L_KICK_DAM;
            break;

        case 7: // hard kick
            p1->isDamaged += H_KICK_DAM;
            break;

        // // // //
        // player 2
        case 8: // cima (ativado apenas se o player estiver onGround)
            if (p2->onGround) {
                p2->onGround = 0;
                p2->vy = -JUMP_VEL;
            }
            break;
            
        case 9: // esquerda
            p2->vx = -MOVE_STEP;
            if (checkWallCollision(p2))
                p2->bodyHitbox->x = 0;
            else if (checkPlayerCollision(p2, p1)) {
                if (checkWallCollision(p1)) {
                    p1->bodyHitbox->x = 0;
                    p2->bodyHitbox->x = p2->bodyHitbox->width + 5;
                }
                else {
                    p1->vx = p2->vx + 7;
                    p1->bodyHitbox->x += p1->vx;
                    p2->bodyHitbox->x += p2->vx + 7;
                }
            }
            else {
                p2->bodyHitbox->x += p2->vx;
            }
            p2->vx = 0;
            break;
            
            
        case 10: // baixo
            if (p2->onGround)
                if ((p2->isCrouch == 1)){
                    p2->bodyHitbox->height = P_CROUCH_HEIGHT;
                    p2->bodyHitbox->y = Y_GROUND - p2->bodyHitbox->height;
                }
                else {
                    p2->bodyHitbox->height = P_HEIGHT;
                    p2->bodyHitbox->y = Y_GROUND - p2->bodyHitbox->height;
                    p2->getUp = 0;
                }
            break;
            
        case 11: // direita
            p2->vx = MOVE_STEP;
            if (checkWallCollision(p2))
                p2->bodyHitbox->x = X_SCREEN - p2->bodyHitbox->width;
            else if (checkPlayerCollision(p2, p1)) {
                if (checkWallCollision(p1)) {
                    p1->bodyHitbox->x = X_SCREEN - p1->bodyHitbox->width;
                    p2->bodyHitbox->x = X_SCREEN - (p2->bodyHitbox->width * 2) - 5;
                }
                else {
                    p1->vx = p2->vx - 7;
                    p1->bodyHitbox->x += p1->vx;
                    p2->bodyHitbox->x += p2->vx - 7;
                }
            }
            else {
                p2->bodyHitbox->x += p2->vx;
            }
            p2->vx = 0;
            break;
            
        case 12: // light punch
            p2->isDamaged += L_PUNCH_DAM;
            break;

        case 13: // hard punch
            p2->isDamaged += H_PUNCH_DAM;
            break;

        case 14: // light kick
            p2->isDamaged += L_KICK_DAM;
            break;

        case 15: // hard kick
            p2->isDamaged += H_KICK_DAM;
            break;
    }
}

void playerDestroy (player *element) {
    if (element) {
        joystickDestroy (element->action);
        hitboxDestroy (element->bodyHitbox);
        if (element->sprite)
            al_destroy_bitmap(element->sprite);
    }
    free (element);
}