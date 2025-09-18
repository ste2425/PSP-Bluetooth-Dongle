#include "commands_map.h"

// Command to get type and battery level of the controller at the specified index.
void controllerInfoExecute(void) {
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

    auto model = controller->getModel();
    auto batteryLevel = controller->battery();

    SerialWrapper_write(RESPONSE_INFO_OK);
    SerialWrapper_write(model);
    SerialWrapper_write(batteryLevel);
}