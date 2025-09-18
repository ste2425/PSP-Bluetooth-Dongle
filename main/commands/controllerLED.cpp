#include "commands_map.h"

// Command to set the LEF colour of a connected controller. Recieves 3 extra btytes of RGB data
void controllerLEDExecute(void) {
    uint8_t payload[4] = {0};
    auto bytesRead = SerialWrapper_readBytes((char*)payload, 4);

    if (bytesRead != 4) {
        SerialWrapper_write(DATA_NOT_RECIEVED);
        return;
    }

    auto controller = controlerManagerGetController(payload[0]);

    if (controller == nullptr) {
        SerialWrapper_write(CONTROLLER_NOT_FOUND);
        return;
    }

    uint8_t buf[1] = { RESPONSE_LED_OK};
    SerialWrapper_write(buf, 1);

    // TODO move this into second core so not to block incoming commands
    controller->setColorLED(payload[1], payload[2], payload[3]);
}