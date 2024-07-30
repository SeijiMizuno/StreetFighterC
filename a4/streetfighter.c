// gcc -g streetfighter.c player.c joystick.c hitbox.c gameinfo.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "player.h"
#include "gameinfo.h"


void updatePlayer (player *p1, player *p2) {
    if (p1->action->up) {
        if (p1->airStun == 0) {
            if (p1->action->left)
                p1->airStun = -JUMP_STUN;
            else if (p1->action->right)
                p1->airStun = JUMP_STUN;
        }
        playerAction(p1, p2, 0);
        toggleState(&p1->action->up);
    } 
    if (p1->action->left) {
        if (p1->onGround)
            playerAction(p1, p2, 1);
    }
    if (p1->action->down) {
        playerAction(p1, p2, 2);
    }
    else if (p1->getUp) {
        playerAction(p1, p2, 2);
    }
    if (p1->action->right) {
        if (p1->onGround)
            playerAction(p1, p2, 3);
    }
    if (p1->action->light_punch) {
        playerAction(p1, p2, 4);
        toggleState(&p1->action->light_punch);
    }
    if (p1->action->hard_punch) {
        playerAction(p1, p2, 5);
        toggleState(&p1->action->hard_punch);
    }
    if (p1->action->light_kick) {
        playerAction(p1, p2, 6);
        toggleState(&p1->action->light_kick);
    }
    if (p1->action->hard_kick) {
        playerAction(p1, p2, 7);
        toggleState(&p1->action->hard_kick);
    }

    if (p2->action->up) {
        if (p2->airStun == 0) {
            if (p2->action->left)
                p2->airStun = -JUMP_STUN;
            else if (p2->action->right)
                p2->airStun = JUMP_STUN;
        }
        playerAction(p1, p2, 8);
        toggleState(&p2->action->up);
    } 
    if (p2->action->left) {
        if (p2->onGround)
            playerAction(p1, p2, 9);
    }
    if (p2->action->down) {
        playerAction(p1, p2, 10);
    }
    else if (p2->getUp) {
        playerAction(p1, p2, 10);
    }
    if (p2->action->right) {
        if (p2->onGround)
            playerAction(p1, p2, 11);
    }
    if (p2->action->light_punch) {
        playerAction(p1, p2, 12);
        toggleState(&p2->action->light_punch);
    }
    if (p2->action->hard_punch) {
        playerAction(p1, p2, 13);
        toggleState(&p2->action->hard_punch);
    }
    if (p2->action->light_kick) {
        playerAction(p1, p2, 14);
        toggleState(&p2->action->light_kick);
    }
    if (p2->action->hard_kick) {
        playerAction(p1, p2, 15);
        toggleState(&p2->action->hard_kick);
    }
}

void processKeyboard (int keycode, player *p1, player *p2) {
    switch (keycode) {
        // player 1
        case ALLEGRO_KEY_A:
            joystickLeft(p1->action);
            break;

        case ALLEGRO_KEY_W:
            if (p1->action->pressedUp == 0) {
                joystickUp(p1->action);
                if (p1->onGround)
                    toggleState(&p1->onGround);
                printf("W\n");
            }
            toggleState(&p1->action->pressedUp);
            break;

        case ALLEGRO_KEY_S:
            joystickDown(p1->action);
            toggleState(&p1->isCrouch);
            if (p1->isCrouch == 0 && p1->onGround)
                p1->getUp = 1;
            break;

        case ALLEGRO_KEY_D:
            joystickRight(p1->action);
            break;
        
        case ALLEGRO_KEY_U:
            if (p1->action->pressedLP == 0 && p1->atkCooldown == 0) {
                setAtkCooldown (p1);
                joystickLightPunch(p1->action);
            }
            toggleState(&p1->action->pressedLP);
            break;
        
        case ALLEGRO_KEY_I:
            if (p1->action->pressedHP == 0 && p1->atkCooldown == 0) {
                setAtkCooldown (p1);
                joystickHardPunch(p1->action);
            }
            toggleState(&p1->action->pressedHP);
            break;

        case ALLEGRO_KEY_J:
            if (p1->action->pressedLK == 0 && p1->atkCooldown == 0) {
                setAtkCooldown (p1);
                joystickLightKick(p1->action);
            }
            toggleState(&p1->action->pressedLK);
            break;
        
        case ALLEGRO_KEY_K:
            if (p1->action->pressedHK == 0 && p1->atkCooldown == 0) {
                setAtkCooldown (p1);
                joystickHardKick(p1->action);
            }
            toggleState(&p1->action->pressedHK);
            break;


        // player 2
        case 82: // left arrow
            joystickLeft(p2->action);
            break;

        case 84: // up arrow
            joystickUp(p2->action);
            if (p2->onGround)
                toggleState(&p2->onGround);
            break;

        case 85: // down arrow
            joystickDown(p2->action);
            toggleState(&p2->isCrouch);
            if (p2->isCrouch == 0 && p2->onGround)
                p2->getUp = 1;
            break;

        case 83: // right arrow
            joystickRight(p2->action);
            break;
        
        case ALLEGRO_KEY_PAD_5:
            if (p2->action->pressedLP == 0 && p2->atkCooldown == 0) {
                setAtkCooldown (p2);
                joystickLightPunch(p2->action);
            }
            toggleState(&p2->action->pressedLP);
            break;
        
        case ALLEGRO_KEY_PAD_6:
            if (p2->action->pressedHP == 0 && p2->atkCooldown == 0) {
                setAtkCooldown (p2);
                joystickHardPunch(p2->action);
            }
            toggleState(&p2->action->pressedHP);
            break;

        case ALLEGRO_KEY_PAD_2:
            if (p2->action->pressedLK == 0 && p2->atkCooldown == 0) {
                setAtkCooldown (p2);
                joystickLightKick(p2->action);
            }
            toggleState(&p2->action->pressedLK);
            break;
        
        case ALLEGRO_KEY_PAD_3:
            if (p2->action->pressedHK == 0 && p2->atkCooldown == 0) {
                setAtkCooldown (p2);
                joystickHardKick(p2->action);
            }
            toggleState(&p2->action->pressedHK);
            break;
    }
}

void drawPlayer (player *p, unsigned char enableHitbox) {
    // float spriteW = al_get_bitmap_width(p->sprite);
    // float spriteH = al_get_bitmap_height(p->sprite);
    ALLEGRO_COLOR cor;
    ALLEGRO_COLOR vermelho = al_map_rgb(255,0,0);

    if (p->character_id == 0)
        cor = al_map_rgb (0,255,0);
    else cor = al_map_rgb (0,0,255);
    if (enableHitbox) {
        al_draw_rectangle(p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->x + p->bodyHitbox->width, p->bodyHitbox->y + p->bodyHitbox->height, cor, 1);
        al_draw_rectangle(p->bodyHitbox->x + p->bodyHitbox->width/2, p->bodyHitbox->y, p->bodyHitbox->x + p->bodyHitbox->width/2, p->bodyHitbox->y + p->bodyHitbox->height, vermelho, 1);
        if (p->facing == 0)
            al_draw_rectangle(p->bodyHitbox->x + p->bodyHitbox->width - 5, p->bodyHitbox->y + 20, p->bodyHitbox->x + p->bodyHitbox->width - 1, p->bodyHitbox->y + 30, cor, 1);
        else
            al_draw_rectangle(p->bodyHitbox->x + 1, p->bodyHitbox->y + 20, p->bodyHitbox->x + 5, p->bodyHitbox->y + 30, cor, 1);
    }

    // al_draw_rectangle(p->bodyHitbox->x + p->bodyHitbox->width, p->bodyHitbox->y + 30, p->bodyHitbox->x + p->bodyHitbox->width + L_PUNCH_W, p->bodyHitbox->y + 30 + PUNCH_H, al_map_rgb(255,0,0), 1);
    // al_draw_scaled_bitmap(p->sprite, 0, 0, spriteW, spriteH, p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->width, p->bodyHitbox->height, 0);
    // al_draw_scaled_bitmap(p->sprite, 0, 0, spriteW, spriteH, p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->width + spriteW, p->bodyHitbox->height, 0);

}

int main () {
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer (1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

    player *p1 = playerCreate(INIT_X1_POS, Y_GROUND-P_HEIGHT, P_WIDTH, P_HEIGHT);
    player *p2 = playerCreate(INIT_X2_POS, Y_GROUND-P_HEIGHT, P_WIDTH, P_HEIGHT);
    
    // inicialização padrão do player2
    p2->character_id = 1;
    p2->facing = 1;
    p2->isHuman = 0;
    
    gameStatus *gameStatus = gameStatusCreate();


    ALLEGRO_EVENT event;

	al_start_timer(timer);
    
    // p1->sprite = al_load_bitmap("./ryu.png");
    p1->sprite = al_load_bitmap("./img/ryuLPunch.png");
    ALLEGRO_COLOR imgBackgroundColor = al_get_pixel(p1->sprite, 0, 0);
    al_convert_mask_to_alpha(p1->sprite, imgBackgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    while (1) {
        al_wait_for_event(queue, &event);

    // printf("--%d--\n", checkWallCollision(p2));
    // printf("p1_x1 == %3d; p2_x1 == %d\n",p1->bodyHitbox->x, p2->bodyHitbox->x);
    // printf("p1_x2 == %3d; p2_x2 == %d\n",p1->bodyHitbox->x + p1->bodyHitbox->width, p2->bodyHitbox->x + p2->bodyHitbox->width);
    // printf("p1_y1 == %3d; p2_y1 == %d\n",p1->bodyHitbox->y, p2->bodyHitbox->y);
    // printf("p1_y2 == %3d; p2_y2 == %d\n",p1->bodyHitbox->y + p2->bodyHitbox->height, p2->bodyHitbox->y + p2->bodyHitbox->height);

    // printf("p1_y2 > p2_y1 == %d\n",p1->bodyHitbox->y + p1->bodyHitbox->height > p2->bodyHitbox->y);
    // printf("p1_y1 < p2_y2 == %d\n",p1->bodyHitbox->y < p2->bodyHitbox->y + p2->bodyHitbox->height);
    // printf("&& -> %d\n\n",(p1->bodyHitbox->y + p1->bodyHitbox->height > p2->bodyHitbox->y) && (p1->bodyHitbox->y < p2->bodyHitbox->y + p2->bodyHitbox->height));
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(10,10,20));

            updateGravity(p1);
            updateGravity(p2);

            updateJump(p1, p2);
            updatePlayer(p1, p2);
            updateFacing(p1, p2);
            separatePlayers(p1, p2);

            updateLifeBar(p1, p2);
            
            updateAtkCooldown(p1);
            
            al_draw_rectangle(0, Y_GROUND + 2, X_SCREEN, Y_GROUND + 2, al_map_rgb(255,0,0), 1); // desenho da linha do chão
            
            drawPlayer(p1, 1);
            drawPlayer(p2, 1);
            al_flip_display();
    // printf("%d p1 y + h\n%d p2 y\n\n", p1->bodyHitbox->y + p1->bodyHitbox->height, p2->bodyHitbox->y);

        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;
            processKeyboard (event.keyboard.keycode, p1, p2);
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    gameStatusDestroy(gameStatus);
    playerDestroy(p1);

    return 0;
}