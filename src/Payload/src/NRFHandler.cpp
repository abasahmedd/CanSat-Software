
#include "NRFHandler.h"
#include <SPI.h>
#include <RF24.h>

RF24 radio(2, 15); // CE, CSN

struct PressureData {
    float pressure;
    float altitude;
};

void initNRF() {
    radio.begin();
    radio.setPALevel(RF24_PA_HIGH);
    radio.setChannel(108);
    radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
    radio.startListening();
}

bool receivePressure(float* pressure, float* alt) {
    if (radio.available()) {
        PressureData data;
        radio.read(&data, sizeof(data));
        *pressure = data.pressure;
        *alt = data.altitude;
        return true;
    }
    return false;
}
