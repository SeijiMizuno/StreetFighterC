#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "hitbox.h"
#include "joystick.h"

#define MOVE_STEP 10
#define JUMP_VEL 45
// #define JUMP_VEL 20
#define JUMP_STUN 22

#define P_WIDTH 116
#define P_HEIGHT 218
#define P_CROUCH_HEIGHT P_HEIGHT / 2

#define L_PUNCH_DAM 30
#define H_PUNCH_DAM 35
#define L_KICK_DAM 40
#define H_KICK_DAM 45

#define L_PUNCH_W 110
#define H_PUNCH_W 130
#define PUNCH_H 100
#define L_KICK_W 130
#define H_KICK_W 140
#define KICK_H 120

#define ATK_TICK 5
#define SUPER_HADOUKEN_COOLDOWN 20
#define SUPER_HADOUKEN_DAM 1

#define MAX_COMBOS 3
#define MAX_COMBO_KEY 15

#define COMBO_TIME_LIMIT 10
#define ATK_COOLDOWN 15

typedef struct {
    unsigned char character_id;
    signed short hp;
    signed char vx, vy;
    signed char airStun;            // velocidade do jogador quando está preso em um movimento horizontal no ar
    unsigned char onGround;         // flag que indica se o jogador está no chão;
    unsigned char isCrouch;         // flag que indica se está em pé (0) ou agachado (1)
    unsigned char roundWin;         // guarda a quantidade de rounds ganhos pelo player (se roundWin == 2, fim da partida)
    unsigned short isDamaged;       // flag que indica se um jogador está "ferido". Enquanto um jogador estiver ferido, sua barra de vida decai
    unsigned char atkHitboxTick;    // contador que indica tempo de duração da hitbox de um ataque. Se um ataque é efetuado, o contador é setado para ATK_HITBOX_TICK
    unsigned char atkCooldown;      // contador que evita spam de ataques, sempre que algum ataque é chamado, esse contador é setado para ATK_COOLDOWN
    unsigned char comboCooldown;    // contador que evita spam de combos, sempre que algum combo é efetuado, esse contador é setado para [COMBO_NAME]_COOLDOWN;
    unsigned char isHuman;          // flag que indica se o jogador é humano ou bot (se isHuman == 1, jogador é humano);
    unsigned char facing;           // flag que indica o lado que o jogador está olhando (se facing == 1, jogador está olhando para esquerda);
    unsigned short comboIndex;
    unsigned short comboTimeElapsed;
    unsigned short comboDamage;
    int comboSuccess;               // flag que indica se o jogador efetuou um combo;
    int *combo;
    ALLEGRO_BITMAP *sprite;
    joystick *action;
    hitbox *bodyHitbox;
    hitbox *atkHitbox;
    hitbox *comboHitbox;
} player;

player *playerCreate (unsigned short init_x, unsigned short init_y, unsigned short widht, unsigned short height);
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
void setAtkHitbox (player *element);
void insertComboKey (player *p, int keycode);
void checkCombo (player *p, int keycode);
void updateCombo (player *p1, player *p2);
void playerAction (player *p1, player *p2, unsigned char action);
void playerDestroy (player *element);

#endif