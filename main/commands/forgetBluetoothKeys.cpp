#include "commands_map.h"

void forgetBluetoothKeysExecute(void) {
    controllerManagerForgetBluetoothKeys();
    SerialWrapper_write(RESPONSE_FORGETKEYS_OK);
}