// gcc -g streetfighter.c player.c joystick.c hitbox.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "player.h"
#include "cenario.h"


void updatePlayer (player *player) {
    if (player->action->up) {
        playerAction(player, 0);
    } 
    if (player->action->left) {
        playerAction(player, 1);
    }
    if (player->action->down) {
        playerAction(player, 2);
    }
    else if (player->getUp) {
        playerAction(player, 2);
    }
    if (player->action->right) {
        playerAction(player, 3);
    }
    if (player->action->light_punch) {
        playerAction(player, 4);
        toggleState(&player->action->light_punch);
    }
    if (player->action->hard_punch) {
        playerAction(player, 5);
        toggleState(&player->action->hard_punch);
    }
    if (player->action->light_kick) {
        playerAction(player, 6);
        toggleState(&player->action->light_kick);
    }
    if (player->action->hard_kick) {
        playerAction(player, 7);
        toggleState(&player->action->hard_kick);
    }
}

void processKeyboard (int keycode, player *p1) {
    switch (keycode) {
        case ALLEGRO_KEY_A:
            joystickLeft(p1->action);
            break;

        case ALLEGRO_KEY_W:
            joystickUp(p1->action);
            if (p1->onGround)
                toggleState(&p1->onGround);
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
    }
}

void drawPlayer (player *p, unsigned char enableHitbox) {
    if (enableHitbox) {
        if (p->isCrouch && p->onGround) {
            al_draw_rectangle(p->hitbox->x, p->hitbox->y, p->hitbox->x + p->hitbox->width, p->hitbox->y + p->hitbox->crouchHeight, al_map_rgb(0,255,0), 1);
        }
        else {
            al_draw_rectangle(p->hitbox->x, p->hitbox->y, p->hitbox->x + p->hitbox->width, p->hitbox->y + p->hitbox->height, al_map_rgb(0,255,0), 1);
        }


    } // adicionar função else, para não exibir hitbox;
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

    ALLEGRO_EVENT event;

	al_start_timer(timer);
    
    p1->sprite = al_load_bitmap("./ryu.png");
    ALLEGRO_COLOR imgBackgroundColor = al_get_pixel(p1->sprite, 0, 0);
    al_convert_mask_to_alpha(p1->sprite, imgBackgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    float spriteW = al_get_bitmap_width(p1->sprite);
    float spriteH = al_get_bitmap_height(p1->sprite);

    while (1) {
        al_wait_for_event(queue, &event);
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0,0,10));
            updateGravity(p1);
            updatePlayer(p1);
            updateLifeBar(p1);
            updateAtkCooldown(p1);
            al_draw_rectangle(0, Y_GROUND, X_SCREEN, Y_GROUND, al_map_rgb(255,0,0), 1);
            al_draw_scaled_bitmap(p1->sprite, 0, 0, spriteW, spriteH, p1->hitbox->x, p1->hitbox->y, p1->hitbox->width, p1->hitbox->height, 0);
            drawPlayer(p1, 1);
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;
            processKeyboard (event.keyboard.keycode, p1);
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    playerDestroy(p1);

    return 0;
}