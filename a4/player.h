#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "hitbox.h"
#include "joystick.h"

#define MOVE_STEP 10
#define JUMP_VEL 20

#define P_WIDTH 116
#define P_HEIGHT 218

#define L_PUNCH_DAM 30
#define H_PUNCH_DAM 35
#define L_KICK_DAM 40
#define H_KICK_DAM 45

#define L_PUNCH_W 65
#define L_PUNCH_H 35
#define H_PUNCH_W 0
#define H_PUNCH_H 0
#define L_KICK_W 0
#define L_KICK_H 0
#define H_KICK_W 0
#define H_KICK_H 0
// SHOULDER Y = BODYHITBOX Y + 30

#define ATK_COOLDOWN 10

typedef struct {
    unsigned char character_id;
    signed short hp;
    signed char vx, vy;
    unsigned char onGround;
    unsigned char isCrouch;     // flag que indica se está em pé (0) ou agachado (1)
    unsigned char getUp;        // flag que indica se é necessário se levantar (ativado quando há o KEY_UP da tecla 's')
    unsigned char roundWin;     // guarda a quantidade de rounds ganhos pelo player (se roundWin == 2, fim da partida)
    unsigned short isDamaged;   // flag que indica se um jogador está "ferido". Enquanto um jogador estiver ferido, sua barra de vida decai
    unsigned char atkCooldown;  // contador que evita spam de ataques, sempre que algum ataque é chamado, esse contador é setado para ATK_COOLDOWN
    unsigned char isHuman;      // flag que indica se o jogador é humano ou bot (se isHuman == 1, jogador é humano);
    unsigned char facing;       // flag que indica o lado que o jogador está olhando (se facing == 1, jogador está olhando para esquerda);
    ALLEGRO_BITMAP *sprite;
    joystick *action;
    hitbox *bodyHitbox;
} player;

player *playerCreate (unsigned short init_x, unsigned short init_y, unsigned short widht, unsigned short height, unsigned char isHuman);
void toggleState (unsigned char *toBeToggled);
/***
 * nota: 'action' se refere à ação que deverá ser efetuada.
 * para 'action' igual à:
 * 0: pular
 * 1: mover esq.
 * 2: agachar
 * 3: mover direita.
 * 
 * o valor 100 é utilizado para tratamento de movimentos além dos controles do jogador,
 * como, por exemplo, a gravidade.
 */
void setAtkCooldown (player *player);
void updateAtkCooldown (player *player);
void playerAction (player *element, unsigned char action);
void playerDestroy (player *element);

#endif