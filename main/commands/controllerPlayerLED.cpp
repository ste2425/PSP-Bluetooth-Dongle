#include "commands_map.h"

void controllerPlayerLEDExecute(void) {
    uint8_t payload[2] = {0};
    auto bytesRead = SerialWrapper_readBytes((char*)payload, 2);

    if (bytesRead != 2) {
        SerialWrapper_write(DATA_NOT_RECIEVED);
        return;
    }

    auto controller = controlerManagerGetController(payload[0]);

    if (controller == nullptr) {
        SerialWrapper_write(CONTROLLER_NOT_FOUND);
        return;
    }

    uint8_t buf[1] = { RESPONSE_PLAYERLED_OK};
    SerialWrapper_write(buf, 1);

    controller->setPlayerLEDs(payload[1]);
}