#include "commands_map.h"

// Command to disconnect a connected controller at the specified index.
void disconnectControllerExecute(void) {
    uint8_t index = 0;
    auto bytesRead = SerialWrapper_readBytes((char*)&index, 1);

    if (bytesRead != 1) {
        SerialWrapper_write(DATA_NOT_RECIEVED);
        return;
    }

    auto controller = controlerManagerGetController(index);

    if (controller == nullptr) {
        SerialWrapper_write(CONTROLLER_NOT_FOUND);
        return;
    }

    controller->disconnect();
    
    SerialWrapper_write(RESPONSE_DISCONNECT_OK);
}