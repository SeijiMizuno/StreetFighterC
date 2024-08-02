#ifndef __JOYSTICK__
#define __JOYSTICK__

typedef struct {
    unsigned char right;
    unsigned char left;
    unsigned char up;
    unsigned char down;
    unsigned char light_punch;
    unsigned char hard_punch;
    unsigned char light_kick;
    unsigned char hard_kick;
} joystick;

joystick* joystickCreate();
void joystickDestroy(joystick *action);
void joystickRight(joystick *action);
void joystickLeft(joystick *action);
void joystickUp(joystick *action);
void joystickDown(joystick *action);
void joystickLightPunch(joystick *action);
void joystickHardPunch(joystick *action);
void joystickLightKick(joystick *action);
void joystickHardKick(joystick *action);

#endif