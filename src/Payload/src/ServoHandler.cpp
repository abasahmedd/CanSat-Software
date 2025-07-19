
#include "ServoHandler.h"

Servo servo1;
Servo servo2;

int getAngle(char code) {
    switch (code) {
        case 'R': return 0;
        case 'G': return 90;
        case 'B': return 180;
        case 'N': return 270;
        default: return 270;
    }
}

void initServos() {
    servo1.attach(25); // GPIO25
    servo2.attach(26); // GPIO26
    resetDisks();
}

void moveDisk1(char filter) {
    int angle = getAngle(filter);
    servo1.write(angle);
}

void moveDisk2(char filter) {
    int angle = getAngle(filter);
    servo2.write(angle);
}

void resetDisks() {
    servo1.write(270);
    servo2.write(270);
}
