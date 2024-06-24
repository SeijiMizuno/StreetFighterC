#ifndef __HITBOX__
#define __HITBOX__

typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
    unsigned short crouchHeight; // static height from the ground to the top of the player when crouched
} hitbox;

hitbox* hitboxCreate(unsigned short init_x, unsigned short init_y, unsigned short max_x, unsigned short max_y);
void hitboxDestroy (hitbox *element);

#endif