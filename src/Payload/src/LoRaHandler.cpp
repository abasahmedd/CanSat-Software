
#include "LoRaHandler.h"
#include <LoRa.h>

void initLoRa() {
    LoRa.begin(915E6);
}

bool receiveGCSCommand(char* cmd) {
    int packetSize = LoRa.parsePacket();
    if (packetSize > 0) {
        int i = 0;
        while (LoRa.available() && i < 4) {
            cmd[i++] = (char)LoRa.read();
        }
        cmd[i] = '\0';
        return true;
    }
    return false;
}
