#include "commands_map.h"

// Command to enable new Bluetooth connections
void enableNewConnectionsExecute(void) {
    controllerManagerEnableNewConnections();
    SerialWrapper_write(RESPONSE_NEWCON_OK);
}

// Command to disable new Bluetooth connections
void disableNewConnectionsExecute(void) {
    controllerManagerDisableNewConnections();
    SerialWrapper_write(RESPONSE_DISNEWCON_OK);
}