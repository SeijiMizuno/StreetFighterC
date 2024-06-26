void processKeyboard (int keycode, player *p1, player *p2) {
    switch (keycode) {
        // player 1
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

    if (enableHitbox) {
        if (p->isCrouch && p->onGround) {
            al_draw_rectangle(p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->x + p->bodyHitbox->width, p->bodyHitbox->y + p->bodyHitbox->crouchHeight, al_map_rgb(0,255,0), 1);
        }
        else {
            al_draw_rectangle(p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->x + p->bodyHitbox->width, p->bodyHitbox->y + p->bodyHitbox->height, al_map_rgb(0,255,0), 1);
        }
    }

    // al_draw_scaled_bitmap(p->sprite, 0, 0, spriteW, spriteH, p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->width, p->bodyHitbox->height, 0);
    // al_draw_scaled_bitmap(p->sprite, 0, 0, spriteW, spriteH, p->bodyHitbox->x, p->bodyHitbox->y, p->bodyHitbox->width + spriteW, p->bodyHitbox->height, 0);
    // al_draw_rectangle(p->bodyHitbox->x + p->bodyHitbox->width, p->bodyHitbox->y + 30, p->bodyHitbox->x + p->bodyHitbox->width + L_PUNCH_W, p->bodyHitbox->y + 30 + L_PUNCH_H, al_map_rgb(255,0,0), 1);

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

    player *p1 = playerCreate(INIT_X1_POS, Y_GROUND-P_HEIGHT, P_WIDTH, P_HEIGHT, 1);
    player *p2 = playerCreate(INIT_X2_POS, Y_GROUND-P_HEIGHT, P_WIDTH, P_HEIGHT, 0);
    gameStatus *gameStatus = gameStatusCreate();

    ALLEGRO_EVENT event;

	al_start_timer(timer);
    
    // p1->sprite = al_load_bitmap("./ryu.png");
    p1->sprite = al_load_bitmap("./ryuLPunch.png");
    ALLEGRO_COLOR imgBackgroundColor = al_get_pixel(p1->sprite, 0, 0);
    al_convert_mask_to_alpha(p1->sprite, imgBackgroundColor);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    while (1) {
        al_wait_for_event(queue, &event);
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(0,0,10));
            updateGravity(p1);
            updateGravity(p2);

            updatePlayer(p1);
            updatePlayer(p2);
            
            updateLifeBar(p1);
            updateAtkCooldown(p1);
            al_draw_rectangle(0, Y_GROUND, X_SCREEN, Y_GROUND, al_map_rgb(255,0,0), 1); // desenho da linha do chão
            drawPlayer(p1, 1);
            drawPlayer(p2, 1);
            al_flip_display();
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