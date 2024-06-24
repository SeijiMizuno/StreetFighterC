#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "hitbox.h"
#include "joystick.h"
#include "cenario.h"

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
    ret_player->getUp = 0;
    ret_player->roundWin = 0;
    ret_player->isDamaged = 0;
    ret_player->atkCooldown = 0;
    ret_player->sprite = NULL;
    ret_player->action = joystickCreate();
    ret_player->hitbox = hitboxCreate(init_x, init_y, widht, height);

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

void playerAction (player *element, unsigned char action) {
    switch (action) {
        case 0: // cima (ativado apenas se o player estiver onGround)
            if (element->onGround) {
                element->onGround = 0;
                element->vy = -JUMP_VEL;
            }
            break;
            
        case 1: // esquerda
            element->vx = -MOVE_STEP;
            element->hitbox->x += element->vx;
            element->vx = 0;
            break;
            
        case 2: // baixo
            if (element->onGround)
                if ((element->isCrouch == 1)){
                    if (element->hitbox->y < (Y_GROUND - element->hitbox->crouchHeight))
                        element->hitbox->y = Y_GROUND - element->hitbox->crouchHeight;
                }
                else {
                    element->hitbox->y = Y_GROUND - element->hitbox->height;
                    element->getUp = 0;
                }
            break;
            
        case 3: // direita
            element->vx = MOVE_STEP;
            element->hitbox->x += element->vx;
            element->vx = 0;
            break;
            
        case 4: // light punch
            element->isDamaged += L_PUNCH_DAM;
            break;

        case 5: // hard punch
            element->isDamaged += H_PUNCH_DAM;
            break;

        case 6: // light kick
            element->isDamaged += L_KICK_DAM;
            break;

        case 7: // hard kick
            element->isDamaged += H_KICK_DAM;
            break;
    }
        printf("%d\n", element->isDamaged);
        printf("%d\n", element->hp);
}

void playerDestroy (player *element) {
    if (element) {
        joystickDestroy (element->action);
        hitboxDestroy (element->hitbox);
        if (element->sprite)
            al_destroy_bitmap(element->sprite);
    }
    free (element);
}