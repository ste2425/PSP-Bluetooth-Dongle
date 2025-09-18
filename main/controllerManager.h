#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <Bluepad32.h>

#ifdef __cplusplus
extern "C" {
#endif

extern ControllerPtr controllers[BP32_MAX_GAMEPADS];

void            controllerManagerSetup(void);
void            controllerManagerLoop(void);
void            controllerManagerEnableNewConnections(void);
void            controllerManagerDisableNewConnections(void);
void            controllerManagerForgetBluetoothKeys(void);
void            controllerManagerDisconnectController(uint8_t index);
ControllerPtr   controlerManagerGetController(uint8_t index);

#ifdef __cplusplus
}
#endif
