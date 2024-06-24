#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "cenario.h"
#include "player.h"
#include "hitbox.h"

void updateGravity (player *element) {
    // atualiza posição y do player
    element->hitbox->y += element->vy;
    element->vy += GRAVITY;

    if (element->hitbox->y + element->hitbox->height >= Y_GROUND) {
        element->hitbox->y = Y_GROUND - element->hitbox->height;
        element->vy = 0;
        element->onGround = 1;
    }
}

// void drawLifeBar (unsigned char p1_hp, unsigned char p2_hp) {
void updateLifeBar (player *element) {
    // fundo vermelho da lifebar
    al_draw_filled_rounded_rectangle(X_P1_LIFEBAR, Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,0,0));
    // vida amarela do jogador
    if (element->isDamaged > 0) {
        element->hp -= LIFE_DECAY_SPEED;
        if (element->isDamaged - LIFE_DECAY_SPEED < 0)
            element->isDamaged = 0;
        else 
            element->isDamaged -= LIFE_DECAY_SPEED;
    }
    if (element->hp >= 0){
        al_draw_filled_rounded_rectangle(X_P1_LIFEBAR + (LIFEBAR_W - element->hp), Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 4, 4, al_map_rgb(255,255,0));
    }
    
    // borda da lifebar
    al_draw_rounded_rectangle(X_P1_LIFEBAR, Y_LIFEBAR, X_P1_LIFEBAR + LIFEBAR_W, Y_LIFEBAR + LIFEBAR_H, 3, 3, al_map_rgb(255,255,255), 2.5);
}
