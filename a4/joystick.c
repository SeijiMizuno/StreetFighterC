#include <stdlib.h>

#include "joystick.h"

joystick *joystickCreate() {
    joystick *ret_jst = (joystick*) malloc(sizeof(joystick));

    if (!ret_jst)
        return NULL;

    ret_jst->right = 0;
    ret_jst->left = 0;
    ret_jst->up = 0;
    ret_jst->down = 0;
    ret_jst->light_punch = 0;
    ret_jst->hard_punch = 0;
    ret_jst->light_kick = 0;
    ret_jst->hard_kick = 0;

    return ret_jst;
}

void joystickDestroy(joystick *action) {
    free (action);
    action = NULL;
}

void joystickRight(joystick *action){
    action->right = !action->right;
}	

void joystickLeft(joystick *action){
    action->left = !action->left;
}	

void joystickUp(joystick *action){
    action->up = !action->up;
}	

void joystickDown(joystick *action){
    action->down = !action->down;
}	

void joystickLightPunch(joystick *action){
    action->light_punch = !action->light_punch;
}	

void joystickHardPunch(joystick *action){
    action->hard_punch = !action->hard_punch;
}	

void joystickLightKick(joystick *action){
    action->light_kick = !action->light_kick;
}	

void joystickHardKick(joystick *action){
    action->hard_kick = !action->hard_kick;
}	
