#include "commands_map.h"

// Command to responsd to a ping. Allows the PSP to know if the serial device
// is connected and that it is this module.
void pingExecute(void) {
    SerialWrapper_write(RESPONSE_PING);
}
