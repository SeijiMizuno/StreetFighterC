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
        element->bodyHitbox->height = P_HEIGHT;
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
    if (p2->isDamaged > 0) {
        p2->hp -= LIFE_DECAY_SPEED;
        if (p2->isDamaged - LIFE_DECAY_SPEED < 0)
            p2->isDamaged = 0;
        else 
            p2->isDamaged -= LIFE_DECAY_SPEED;
    }

    if (p1->hp >= 0)
        al_draw_filled_rounded_rectangle(X_P1_LIFEBAR + (LIFEBAR_W - p1->hp), Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,255,0));
    if (p2->hp >= 0)
        al_draw_filled_rounded_rectangle(X_P2_LIFEBAR, Y_LIFEBAR, X_P2_LIFEBAR + p2->hp , Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,255,0));

    // borda da lifebar
    al_draw_rounded_rectangle(X_P1_LIFEBAR, Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 3, 3, al_map_rgb(255,255,255), 2.5);
    al_draw_rounded_rectangle(X_P2_LIFEBAR, Y_LIFEBAR, X_P2_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 3, 3, al_map_rgb(255,255,255), 2.5);
}

void updateAtkCooldown (player *element) {
    if (element->atkCooldown - 1 >= 0)
        element->atkCooldown--;
    else
        element->atkCooldown = 0;
}

void updateComboCooldown (player *element) {
    if (element->comboCooldown - 1 >= 0)
        element->comboCooldown--;
    else
        element->comboCooldown = 0;
}

unsigned char checkAtkHit (player *p1, player *p2) {
    if ((((p1->atkHitbox->y + p1->atkHitbox->height > p2->bodyHitbox->y) &&
    (p1->atkHitbox->y < p2->bodyHitbox->y + p2->bodyHitbox->height)) ||
    ((p2->bodyHitbox->y + p2->bodyHitbox->height > p1->atkHitbox->y) &&
    (p2->bodyHitbox->y < p1->atkHitbox->y + p1->atkHitbox->height))) 
    ||
    (((p2->atkHitbox->y + p2->atkHitbox->height > p1->bodyHitbox->y) &&
    (p2->atkHitbox->y < p1->bodyHitbox->y + p1->bodyHitbox->height)) ||
    ((p1->bodyHitbox->y + p1->bodyHitbox->height > p2->atkHitbox->y) &&
    (p1->bodyHitbox->y < p2->atkHitbox->y + p2->atkHitbox->height))))
    {
        if (p1->facing == 0 && p2->facing == 1) {
            if ((p1->atkHitbox->x + p1->atkHitbox->width) > (p2->bodyHitbox->x))
                return 1;
            if ((p2->atkHitboxTick != 0) &&
                ((p2->atkHitbox->x - p2->atkHitbox->width) < (p1->bodyHitbox->x + p1->bodyHitbox->width)))
                return 1;
        }
        
        else {
            if ((p2->atkHitbox->x + p2->atkHitbox->width) > (p1->bodyHitbox->x))
                return 1;
            if ((p1->atkHitboxTick != 0) &&
                ((p1->atkHitbox->x - p1->atkHitbox->width) < (p2->bodyHitbox->x + p2->bodyHitbox->width)))
                return 1;
        }
    }

    return 0;
}

unsigned char checkComboHit (player *p1, player *p2) {
    if ((((p1->comboHitbox->y + p1->comboHitbox->height > p2->bodyHitbox->y) &&
        (p1->comboHitbox->y < p2->bodyHitbox->y + p2->bodyHitbox->height)) ||
        ((p2->bodyHitbox->y + p2->bodyHitbox->height > p1->comboHitbox->y) &&
        (p2->bodyHitbox->y < p1->comboHitbox->y + p1->comboHitbox->height))) 
        ||
        (((p2->comboHitbox->y + p2->comboHitbox->height > p1->bodyHitbox->y) &&
        (p2->comboHitbox->y < p1->bodyHitbox->y + p1->bodyHitbox->height)) ||
        ((p1->bodyHitbox->y + p1->bodyHitbox->height > p2->comboHitbox->y) &&
        (p1->bodyHitbox->y < p2->comboHitbox->y + p2->comboHitbox->height)))) {

        if (p1->facing == 0 && p2->facing == 1) {
            if ((p1->comboHitbox->x + p1->comboHitbox->width) > (p2->bodyHitbox->x)) {
                return 1;
            }
            if ((p2->comboCooldown != 0) &&
                ((p2->comboHitbox->x - p2->comboHitbox->width) < (p1->bodyHitbox->x + p1->bodyHitbox->width)))
                return 1;
        }
        
        else {
            if ((p2->comboHitbox->x + p2->comboHitbox->width) > (p1->bodyHitbox->x))
                return 1;
            if ((p1->comboCooldown != 0) &&
                ((p1->comboHitbox->x - p1->comboHitbox->width) < (p2->bodyHitbox->x + p2->bodyHitbox->width)))
                return 1;
        }
    }
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
        else
            if ((p2->bodyHitbox->x + p2->bodyHitbox->width - p1->vx) > (p1->bodyHitbox->x))
                return 1;
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
            if ((p1->bodyHitbox->x + p1->bodyHitbox->width / 2) < (p2->bodyHitbox->x + p2->bodyHitbox->width / 2)) {
                p1->vx = -MOVE_STEP;
                p2->vx = MOVE_STEP;
            }
            else if ((p1->bodyHitbox->x + p1->bodyHitbox->width / 2) >= (p2->bodyHitbox->x + p2->bodyHitbox->width / 2)) {
                p1->vx = MOVE_STEP;
                p2->vx = -MOVE_STEP;
            }   
        }
        else if (checkWallCollision(p1) && checkWallCollision(p2)) {
            if (!p1->onGround){
                p2->vx = 0;
                if (p2->bodyHitbox->x < X_SCREEN / 2)
                    p1->vx = MOVE_STEP*2;
                else
                    p1->vx = -MOVE_STEP*2;
            }
            else if (!p2->onGround) {
                p1->vx = 0;
                if (p1->bodyHitbox->x < X_SCREEN / 2)
                    p2->vx = MOVE_STEP*2;
                else
                    p2->vx = -MOVE_STEP*2;
            }
        }
        else if (checkWallCollision(p1)) {
            p1->vx = 0;
            if (p1->bodyHitbox->x < X_SCREEN / 2)
                p2->vx = MOVE_STEP*2;
            else
                p2->vx = -MOVE_STEP*2;
        }
        else if (checkWallCollision(p2)) {
            p2->vx = 0;
            if (p2->bodyHitbox->x < X_SCREEN / 2)
                p1->vx = MOVE_STEP*2;
            else
                p1->vx = -MOVE_STEP*2;
        }
        
        if (p1->bodyHitbox->x + p1->vx < 0)
            p1->bodyHitbox->x = 0;
        else if(p1->bodyHitbox->x + p1->vx > X_SCREEN)
            p1->bodyHitbox->x = X_SCREEN - p1->bodyHitbox->width;
        else
            p1->bodyHitbox->x += p1->vx;

        if (p2->bodyHitbox->x + p2->vx < 0)
            p2->bodyHitbox->x = 0;
        else if(p2->bodyHitbox->x + p2->vx > X_SCREEN)
            p2->bodyHitbox->x = X_SCREEN - p2->bodyHitbox->width;
        else
            p2->bodyHitbox->x += p2->vx;
    }
    p1->vx = 0;
    p2->vx = 0;
}

void updateComboHitbox (player *p1, player *p2) {
    if (p1->comboCooldown == 0) {
        p1->comboDamage = 0;
        p1->comboHitbox->x = 0;
        p1->comboHitbox->width = 0;
        p1->comboHitbox->y = 0;
        p1->comboHitbox->height = 0;
    }
    else {
        p1->atkHitbox->x = 0;
        p1->atkHitbox->width = 0;
        p1->atkHitbox->y = 0;
        p1->atkHitbox->height = 0;
    }
    switch (p1->comboSuccess) {
        case 1:
            if (p1->onGround) {
                p1->comboCooldown = SUPER_HADOUKEN_COOLDOWN;
                p1->comboDamage = SUPER_HADOUKEN_DAM;
                p1->comboHitbox->y = Y_GROUND - P_HEIGHT;
                p1->comboHitbox->height = P_HEIGHT;
                if (p1->facing == 0) {
                    p1->comboHitbox->x = p1->bodyHitbox->x + p1->bodyHitbox->width;
                    p1->comboHitbox->width = X_SCREEN - (p1->bodyHitbox->x + p1->bodyHitbox->width);
                }
                else {
                    p1->comboHitbox->x = p1->bodyHitbox->x;
                    p1->comboHitbox->width = p1->bodyHitbox->x;
                }
            }
            break;
        case 2:
            break;
        case 3:
            break;
    }
    if (checkComboHit(p1, p2)) {
        printf("%d\n",p1->comboDamage);
        p2->isDamaged = p1->comboDamage;
    }

}

void updateAtkHitbox (player *p1, player *p2) { // para simplificar os textos
    if (p1->atkHitboxTick != 0)
        p1->atkHitboxTick--;
    if (p2->atkHitboxTick != 0)
        p2->atkHitboxTick--;

    if (p1->atkHitboxTick == 0) {
        p1->atkHitbox->x = 0;
        p1->atkHitbox->width = 0;
        p1->atkHitbox->y = 0;
        p1->atkHitbox->height = 0;
    }
    else {
        if (p1->facing == 0)
            p1->atkHitbox->x = p1->bodyHitbox->x + p1->bodyHitbox->width;
        else
            p1->atkHitbox->x = p1->bodyHitbox->x;

        if (p1->onGround)
            if (p1->atkHitbox->height == PUNCH_H || p1->isCrouch)
                p1->atkHitbox->y = p1->bodyHitbox->y;
            else 
                p1->atkHitbox->y = p1->bodyHitbox->y + P_CROUCH_HEIGHT;
        else
            p1->atkHitbox->y = p1->bodyHitbox->y + p1->bodyHitbox->height / 2;
    }
    
    if (p2->atkHitboxTick == 0) {
        p2->atkHitbox->x = 0;
        p2->atkHitbox->width = 0;
        p2->atkHitbox->y = 0;
        p2->atkHitbox->height = 0;
    }
    else {
        if (p2->facing == 0)
            p2->atkHitbox->x = p2->bodyHitbox->x + p2->bodyHitbox->width;
        else
            p2->atkHitbox->x = p2->bodyHitbox->x;

        if (p2->onGround)
            if (p2->atkHitbox->height == PUNCH_H || p2->isCrouch)
                p2->atkHitbox->y = p2->bodyHitbox->y;
            else
                p2->atkHitbox->y = p2->bodyHitbox->y + P_CROUCH_HEIGHT;
        else
            p2->atkHitbox->y = p2->bodyHitbox->y + p2->bodyHitbox->height / 2;
    }
}

void updateJump (player *p1, player *p2) {
    // atualiza posição x do player no ar
    if (p1->airStun) {
        if (p1->airStun < 0) {
            if (!checkWallCollision(p1))
                p1->vx = -MOVE_STEP;
            else if (p1->bodyHitbox->x < (X_SCREEN / 2)) {    // checa se a colisão foi com a parede da direita ou da esquerda
                p1->vx = 0;
                p1->bodyHitbox->x = 0;
            } else
                p1->vx = -MOVE_STEP;
            p1->airStun++;
        }
        else {  // logicamente, airStun > 0
            if (!checkWallCollision(p1))
                p1->vx = MOVE_STEP;
            else if (p1->bodyHitbox->x > X_SCREEN / 2) {    // checa se a colisão foi com a parede da direita ou da esquerda
                p1->vx = 0;
                p1->bodyHitbox->x = X_SCREEN - p1->bodyHitbox->width;
            }
            else
                p1->vx = MOVE_STEP;
            p1->airStun--;
        }
    }
    // atualiza posição x do player no ar
    if (p2->airStun) {
        if (p2->airStun < 0) {
            if (!checkWallCollision(p2))
                p2->vx = -MOVE_STEP;
            else if (p2->bodyHitbox->x < (X_SCREEN / 2)) // checa se a colisão foi com a parede da direita ou da esquerda
                p2->bodyHitbox->x = 0;
            else
                p2->vx = -MOVE_STEP;
            p2->airStun++;
        }
        else {  // logicamente, airStun > 0
            if (!checkWallCollision(p2))
                p2->vx = MOVE_STEP;
            else if (p2->bodyHitbox->x > X_SCREEN / 2) {    // checa se a colisão foi com a parede da direita ou da esquerda
                p2->vx = 0;
                p2->bodyHitbox->x = X_SCREEN - p2->bodyHitbox->width;
            }
            else
                p2->vx = MOVE_STEP;
            p2->airStun--;
        }
    }
    
    if (p1->bodyHitbox->x + p1->vx < 0)
        p1->bodyHitbox->x = 0;
    else
        p1->bodyHitbox->x += p1->vx;

    if (p2->bodyHitbox->x + p2->vx < 0)
        p2->bodyHitbox->x = 0;
    else
        p2->bodyHitbox->x += p2->vx;
    p1->vx = 0;
    p2->vx = 0;
}

void gameStatusDestroy (gameStatus *gameStatus) {
    if (gameStatus)
        free (gameStatus);
} 