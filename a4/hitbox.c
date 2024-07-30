#include <stdlib.h>

#include "hitbox.h"

hitbox* hitboxCreate(unsigned short init_x, unsigned short init_y, unsigned short width, unsigned short height) {
    hitbox *ret_hb = (hitbox*) malloc(sizeof(hitbox));
    
    if (!ret_hb)
        return NULL;
    
    ret_hb->x = init_x;
    ret_hb->y = init_y;
    ret_hb->width = width;
    ret_hb->height = height;

    return ret_hb;
}

void hitboxDestroy (hitbox *element) {
    free (element);
}