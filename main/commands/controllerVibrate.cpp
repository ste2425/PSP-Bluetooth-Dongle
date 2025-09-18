#include "commands_map.h"

// Command to set the vibration of a connected controller. Recieves 7 extra bytes of data
// [index, delayMS (2 bytes), durationMS (2 bytes), largeMotor, smallMotor]
void controllerVibrateExecute(void) {
    uint8_t payload[7] = {0};
    auto bytesRead = SerialWrapper_readBytes((char*)payload, 7);

    if (bytesRead != 7) {
        SerialWrapper_write(DATA_NOT_RECIEVED);
        return;
    }

    auto controller = controlerManagerGetController(payload[0]);

    if (controller == nullptr) {
        SerialWrapper_write(CONTROLLER_NOT_FOUND);
        return;
    }

    uint16_t delayMs = (payload[1] << 8) | payload[2];
    uint16_t durationMS = (payload[3] << 8) | payload[4];

    uint8_t buf[1] = { RESPONSE_VIBRATE_OK };
    SerialWrapper_write(buf, 1);

    // TODO move this into second core so not to block incoming commands
    controller->playDualRumble(delayMs, durationMS, payload[5], payload[6]);
}