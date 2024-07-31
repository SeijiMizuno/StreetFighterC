#ifndef __HITBOX__
#define __HITBOX__

#define ATK_HITBOX_TICK 5

typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
} hitbox;

hitbox* hitboxCreate(unsigned short init_x, unsigned short init_y, unsigned short max_x, unsigned short max_y);
void hitboxDestroy (hitbox *element);

#endif