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

    ret_player->combo = (int*) malloc(MAX_COMBO_KEY * sizeof(int));
    if (!ret_player->combo)
        return NULL;

    for (int i = 0; i < MAX_COMBO_KEY; i++)
        ret_player->combo[i] = 0;
    
    ret_player->character_id = 3;
    ret_player->hp = LIFEBAR_W;
    ret_player->vx = 0;
    ret_player->vy = 0;
    ret_player->onGround = 1;
    ret_player->isCrouch = 0;
    ret_player->airStun = 0;
    ret_player->roundWin = 0;
    ret_player->isDamaged = 0;
    ret_player->atkHitboxTick = 0;
    ret_player->atkCooldown = 0;
    ret_player->comboCooldown = 0;
    ret_player->isHuman = 1;
    ret_player->facing = 0;
    ret_player->comboSuccess = 0;
    ret_player->comboIndex = 0;
    ret_player->comboTimeElapsed = 0;
    ret_player->comboDamage = 0;
    ret_player->sprite = NULL;
    ret_player->action = joystickCreate();
    ret_player->bodyHitbox = hitboxCreate(init_x, init_y, widht, height);
    ret_player->atkHitbox = hitboxCreate(0,0,0,0);
    ret_player->comboHitbox = hitboxCreate(0,0,0,0);

    return ret_player;
}

// sequencia de teclas necessárias quando facing == 0
// MAX_CARACTERS*2 pois são dois jogadores
const int COMBO_SEQUENCES_0[MAX_COMBOS][MAX_COMBO_KEY] = {
    //ryu combo
    // down -> right -> right -> right -> LP -> Down -> Left -> HP
    // p1
    {19, 4, 4, 4, 21, 19, 1, 9, 0, 0, 0, 0, 0, 0, 0},   

    // chun li combo
    // left -> right -> left -> right -> left -> right -> LK -> HK
    // p1
    {1, 4, 1, 4, 1, 4, 10, 11, 0, 0, 0, 0, 0, 0, 0},   

    // blanka combo
    // 
    // p1
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   

    // zangief combo
    // 
    // p1
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},


    //ryu combo
    // down -> right -> right -> right -> LP -> Down -> Left -> HP
    // p2
    {85, 83, 83, 83, 42, 85, 82, 43, 0, 0, 0, 0, 0, 0, 0},   

    // chun li combo
    // left -> right -> left -> right -> left -> right -> LK -> HK
    // p2
    {82, 83, 82, 83, 82, 82, 39, 40, 0, 0, 0, 0, 0, 0, 0},   

    // blanka combo
    // 
    // p2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   

    // zangief combo
    // 
    // p2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// sequencia de teclas necessárias quando facing == 1
const int COMBO_SEQUENCES_1[MAX_COMBOS][MAX_COMBO_KEY] = {
    // ryu combo
    // down -> left -> left -> left -> LP -> down -> right -> HP
    // p1
    {19, 1, 1, 1, 21, 19, 4, 9, 0, 0, 0, 0, 0, 0, 0},   
    
    // chun li combo
    // right -> left -> right -> left -> right -> left -> LK -> HK
    // p1
    {4, 1, 4, 1, 4, 1, 10, 11, 0, 0, 0, 0, 0, 0, 0},   
    
    // blanka combo
    // 
    // p1
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    
    // zangief combo
    // 
    // p1
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},


    // ryu combo
    // down -> left -> left -> left -> LP -> down -> right -> HP
    // p2
    {85, 82, 82, 82, 42, 85, 83, 43, 0, 0, 0, 0, 0, 0, 0},   
    
    // chun li combo
    // right -> left -> right -> left -> right -> left -> LK -> HK
    // p2
    {83, 82, 83, 82, 83, 82, 39, 40, 0, 0, 0, 0, 0, 0, 0},   
    
    // blanka combo
    // 
    // p2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    
    // zangief combo
    // 
    // p2
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void toggleState (unsigned char *toBeToggled) {
    *toBeToggled = !(*toBeToggled);
}

void setAtkCooldown (player *player) {
    if (player->atkCooldown == 0) 
        player->atkCooldown = ATK_COOLDOWN;
}

void setAtkHitbox (player *element) {
    // para simplificar os textos
    unsigned short p_y1;
    unsigned short k_y1;

    if (element->atkHitboxTick == 0) {    
        // decisão da posição y1 do ataque
        if (!element->onGround) {
            p_y1 = element->bodyHitbox->y + (P_CROUCH_HEIGHT / 2);
            k_y1 = element->bodyHitbox->y + (P_CROUCH_HEIGHT / 2);
        }
        else {
            p_y1 = element->bodyHitbox->y;
            if (element->isCrouch)
                k_y1 = element->bodyHitbox->y;
            else
                k_y1 = element->bodyHitbox->y + P_CROUCH_HEIGHT;
        }

        // decisão da posição x1 do ataque
        if (element->facing == 0)
            element->atkHitbox->x = element->bodyHitbox->x + element->bodyHitbox->width;   
        else
            element->atkHitbox->x = element->bodyHitbox->x;

        // inicialização da hitbox de ataque
        if (element->action->light_punch || element->action->hard_punch) {
            element->atkHitbox->y = p_y1;
            element->atkHitbox->height = PUNCH_H;
            if (element->action->light_punch)
                element->atkHitbox->width = L_PUNCH_W;
            else
                element->atkHitbox->width = H_PUNCH_W;
        }
        else if (element->action->light_kick || element->action->hard_kick) {
            element->atkHitbox->y = k_y1;
            element->atkHitbox->height = KICK_H;
            if (element->action->light_kick)
                element->atkHitbox->width = L_KICK_W;
            else
                element->atkHitbox->width = H_KICK_W;
        }
        element->atkHitboxTick = ATK_TICK;
    }
    else
        element->atkHitboxTick--;
}

void insertComboKey (player *p, int keycode) {
    if (p->comboIndex < MAX_COMBO_KEY) {
        p->combo[p->comboIndex] = keycode;
        p->comboTimeElapsed = 0;
        p->comboIndex++;

        checkCombo(p, keycode);
    }
}

void checkCombo (player *p, int keycode) {
    for (int i = 0; i < MAX_COMBO_KEY; i++)
        if (p->facing == 0) {
            if (p->combo[i] == COMBO_SEQUENCES_0[p->character_id][i]) {
                if (COMBO_SEQUENCES_0[p->character_id][i + 1] == 0 && !p->comboCooldown) {
                    p->comboSuccess = p->character_id + 1;
                    return;
                }
            }
            else
                break;
        }
        else {
            if (p->combo[i] == COMBO_SEQUENCES_1[p->character_id][i]) {
                if (COMBO_SEQUENCES_1[p->character_id][i + 1] == 0 && !p->comboCooldown) {
                    p->comboSuccess = p->character_id + 1;
                    return;
                }
            }
            else
                break;
        }

    p->comboSuccess = 0;
}

void updateCombo (player *p1, player *p2) {
    if ((p1->comboTimeElapsed < COMBO_TIME_LIMIT ) && (p1->comboIndex < MAX_COMBO_KEY))
        p1->comboTimeElapsed++;
    else {
        for (int i = 0; i < MAX_COMBO_KEY; i++)
            p1->combo[i] = 0;
        p1->comboIndex = 0;
        p1->comboSuccess = 0;
    }

    if ((p2->comboTimeElapsed < COMBO_TIME_LIMIT ) && (p2->comboIndex < MAX_COMBO_KEY))
        p2->comboTimeElapsed++;
    else {
        for (int i = 0; i < MAX_COMBO_KEY; i++)
            p2->combo[i] = 0;
        p2->comboIndex = 0;
        p2->comboSuccess = 0;
    }
}

void playerAction (player *p1, player *p2, unsigned char action) {
    switch (action) {
        // // // //
        // player 1
        case 0: // cima (ativado apenas se o player estiver onGround e em pé)
            if (p1->airStun == 0) {
                if (p1->action->left)
                    p1->airStun = -JUMP_STUN;
                else if (p1->action->right)
                    p1->airStun = JUMP_STUN;
            }
            if (!p1->isCrouch) {
                if (p1->onGround) {
                    p1->onGround = 0;
                    p1->vy = -JUMP_VEL;
                }
            }
            break;
            
        case 1: // esquerda
            if (!p1->isCrouch && !p1->atkHitboxTick) {
                p1->vx = -MOVE_STEP;
                if (checkWallCollision(p1))
                    p1->bodyHitbox->x = 0;
                else if (checkPlayerCollision(p1, p2)) {
                    if (checkWallCollision(p2) && (p1->bodyHitbox->x < X_SCREEN/2)) {
                        p2->bodyHitbox->x = 0;
                        p1->bodyHitbox->x = (p1->bodyHitbox->width) + 5;
                    }
                    else {
                        p2->vx = p1->vx + 7;
                        if (checkWallCollision(p2) && (p1->bodyHitbox->x < X_SCREEN/2)) {
                            p2->bodyHitbox->x = 0;
                            p1->bodyHitbox->x = (p1->bodyHitbox->width) + 5;
                        } else {
                            p2->bodyHitbox->x += p2->vx;
                            p1->bodyHitbox->x += p1->vx + 7;
                        }
                    }
                }
                else {
                    p1->bodyHitbox->x += p1->vx;
                }
                p2->vx = 0;
                p1->vx = 0;
            }
            break;
            
        case 2: // baixo
            if (p1->isCrouch == 1)
                p1->bodyHitbox->height = P_CROUCH_HEIGHT;
            else
                p1->bodyHitbox->height = P_HEIGHT;
            
            p1->bodyHitbox->y = Y_GROUND - p1->bodyHitbox->height;
            break;
            
        case 3: // direita
            if (!p1->isCrouch && !p1->atkHitboxTick) {
                p1->vx = MOVE_STEP;
                if (checkWallCollision(p1))
                    p1->bodyHitbox->x = X_SCREEN - p1->bodyHitbox->width;
                else if (checkPlayerCollision(p1, p2)) {
                    if (checkWallCollision(p2) && (p1->bodyHitbox->x > X_SCREEN/2)) {
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
            }
            break;
            
        case 4: // light punch
            setAtkHitbox (p1);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown (p1);
                p2->isDamaged += L_PUNCH_DAM;
            }
            break;

        case 5: // hard punch
            setAtkHitbox (p1);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown (p1);
                p2->isDamaged += H_PUNCH_DAM;
            }
            break;

        case 6: // light kick
            setAtkHitbox (p1);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown (p1);
                p2->isDamaged += L_KICK_DAM;
            }
            break;

        case 7: // hard kick
            setAtkHitbox (p1);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown (p1);
                p2->isDamaged += H_KICK_DAM;
            }
            break;

        // // // //
        // player 2
        case 8: // cima (ativado apenas se o player estiver onGround e em pé)
            if (p2->airStun == 0) {
                if (p2->action->left)
                    p2->airStun = -JUMP_STUN;
                else if (p2->action->right)
                    p2->airStun = JUMP_STUN;
            }
            if (!p2->isCrouch) {
                if (p2->onGround) {
                    p2->onGround = 0;
                    p2->vy = -JUMP_VEL;
                }
            }
            break;
            
        case 9: // esquerda
            if (!p2->isCrouch) {
                p2->vx = -MOVE_STEP;
                if (checkWallCollision(p2))
                    p2->bodyHitbox->x = 0;
                else if (checkPlayerCollision(p2, p1)) {
                    if (checkWallCollision(p1) && (p2->bodyHitbox->x < X_SCREEN/2)) {
                        p1->bodyHitbox->x = 0;
                        p2->bodyHitbox->x = p2->bodyHitbox->width + 5;
                    }
                    else {
                        p1->vx = p2->vx + 7;
                        if (checkWallCollision(p1) && (p2->bodyHitbox->x < X_SCREEN/2)) {
                            p1->bodyHitbox->x = 0;
                            p2->bodyHitbox->x = p2->bodyHitbox->width + 5;
                        }
                        else {
                            p1->bodyHitbox->x += p1->vx;
                            p2->bodyHitbox->x += p2->vx + 7;
                        }
                    }
                }
                else {
                    p2->bodyHitbox->x += p2->vx;
                }
                p1->vx = 0;
                p2->vx = 0;
            }
            break;
            
            
        case 10: // baixo
            if (p2->isCrouch == 1)
                p2->bodyHitbox->height = P_CROUCH_HEIGHT;
            else
                p2->bodyHitbox->height = P_HEIGHT;

            p2->bodyHitbox->y = Y_GROUND - p2->bodyHitbox->height;
            break;
            
        case 11: // direita
            if (!p2->isCrouch) {
                p2->vx = MOVE_STEP;
                if (checkWallCollision(p2))
                    p2->bodyHitbox->x = X_SCREEN - p2->bodyHitbox->width;
                else if (checkPlayerCollision(p2, p1)) {
                    if (checkWallCollision(p1) && (p2->bodyHitbox->x > X_SCREEN/2)) {
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
            }
            break;
            
        case 12: // light punch
            setAtkHitbox (p2);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown(p2);
                p1->isDamaged += L_PUNCH_DAM;
            }
            break;

        case 13: // hard punch
            setAtkHitbox (p2);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown(p2);
                p1->isDamaged += H_PUNCH_DAM;
            }
            break;

        case 14: // light kick
            setAtkHitbox (p2);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown(p2);
                p1->isDamaged += L_KICK_DAM;
            }
            break;

        case 15: // hard kick
            setAtkHitbox (p2);
            if(checkAtkHit(p1, p2)) {
                setAtkCooldown(p2);
                p1->isDamaged += H_KICK_DAM;
            }
            break;
    }
    
    // checkAtkHit (p1, p2);
}

void playerDestroy (player *element) {
    if (element) {
        joystickDestroy (element->action);
        hitboxDestroy (element->bodyHitbox);
        hitboxDestroy (element->atkHitbox);
        free (element->combo);
        if (element->sprite)
            al_destroy_bitmap(element->sprite);
    }
    free (element);
}