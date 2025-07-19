
#include "BuzzerHandler.h"
#include <Arduino.h>

#define BUZZER_PIN 27

float prevAlt = 0;
unsigned long lastTime = 0;

void initBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void checkAltitudeForBuzzer() {
    extern float altitude2;

    if (millis() - lastTime > 1000) {
        float delta = prevAlt - altitude2;
        if (delta > 3.0) { // fast descent
            digitalWrite(BUZZER_PIN, HIGH);
            delay(500);
            digitalWrite(BUZZER_PIN, LOW);
        }
        prevAlt = altitude2;
        lastTime = millis();
    }
}
