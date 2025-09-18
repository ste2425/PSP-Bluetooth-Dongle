#include "commands_map.h"

// Command to get the current state of a controller at the specified index.
void controllerDataExecute(void) {
    uint8_t value = 0;
    auto bytesRead = SerialWrapper_readBytes((char*)&value, 1);

    if (bytesRead != 1) {
        SerialWrapper_write(DATA_NOT_RECIEVED);
        return;
    }

    auto controller = controlerManagerGetController(value);

    if (controller == nullptr) {
        SerialWrapper_write(CONTROLLER_NOT_FOUND);
        return;
    }

    uint8_t buf[10];
    
    buf[0] = RESPONSE_OK;
    buf[1] = map(controller->axisRX(), -512, 511, 0, 255);
    buf[2] = map(controller->axisRY(), -512, 511, 0, 255);
    buf[3] = map(controller->axisX(), -512, 511, 0, 255);
    buf[4] = map(controller->axisY(), -512, 511, 0, 255);
    buf[5] = controller->dpad();
    
    auto buttons = controller->buttons();
    buf[6] = (buttons >> 8) & 0xFF;
    buf[7] = buttons & 0xFF;

    auto miscButtons = controller->miscButtons();
    buf[8] = (miscButtons >> 8) & 0xFF;
    buf[9] = miscButtons & 0xFF;

    SerialWrapper_write(buf, 10);
}