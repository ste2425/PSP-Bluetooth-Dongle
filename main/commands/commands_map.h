#pragma once

#include <stdint.h>
#include "../Serial.h"
#include "responses.h"
#include "../controllerManager.h"

// list all command definitions here
void controllerDataExecute(void);
void pingExecute(void);
void enableNewConnectionsExecute(void);
void disableNewConnectionsExecute(void);
void disconnectControllerExecute(void);
void forgetBluetoothKeysExecute(void);
void controllerLEDExecute(void);
void controllerVibrateExecute(void);
void controllerInfoExecute(void);
void controllerPlayerLEDExecute(void);
void firmwareVersionExecute(void);

typedef void (*CommandFunc)(void);

typedef struct {
    uint8_t command;
    CommandFunc func;
} CommandEntry;

// map cmmand functions to command codes
static CommandEntry commandMap[] = {
    {0x02, pingExecute},
    {0x03, controllerDataExecute},
    {0x04, enableNewConnectionsExecute},
    {0x05, disableNewConnectionsExecute},
    {0x06, disconnectControllerExecute},
    {0x07, controllerLEDExecute},
    {0x08, controllerVibrateExecute},
    {0x09, controllerInfoExecute},
    {0xA, forgetBluetoothKeysExecute},
    {0xB, controllerPlayerLEDExecute},
    {0xC, firmwareVersionExecute}
};

static const int commandMapSize = sizeof(commandMap) / sizeof(CommandEntry);
