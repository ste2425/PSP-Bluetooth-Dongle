#include "commandManager.h"

void processCommand(uint8_t command) {
    for (int i = 0; i < commandMapSize; ++i) {
        if (commandMap[i].command == command) {
            commandMap[i].func();
            return;
        }
    }
}