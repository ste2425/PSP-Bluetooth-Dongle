#include "controllerManager.h"

ControllerPtr controllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl);
void onDisconnectedController(ControllerPtr ctl);
void processControllers();
void resetController(uint8_t index);
void processControllers();
void processGamepad(uint8_t index, ControllerPtr ctl);

void controllerManagerSetup(void) {
    bool startScanning = false;
    BP32.setup(&onConnectedController, &onDisconnectedController, startScanning);

    BP32.enableVirtualDevice(false);

    BP32.enableBLEService(false);
}

void controllerManagerForgetBluetoothKeys(void) {
    BP32.forgetBluetoothKeys();
}

void controllerManagerLoop(void) {
    BP32.update();
}

void controllerManagerEnableNewConnections() {
    BP32.enableNewBluetoothConnections(true);
}

void controllerManagerDisableNewConnections(void) {
    BP32.enableNewBluetoothConnections(false);
}

ControllerPtr controlerManagerGetController(uint8_t index) {
    if (index >= BP32_MAX_GAMEPADS)
        return nullptr;

    auto controller = controllers[index];

    if (controller && controller->isConnected())
        return controller;

    return nullptr;
}

void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (controllers[i] == nullptr) {
            controllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (controllers[i] == ctl) {
            controllers[i] = nullptr;
            foundController = true;
            break;
        }
    }
}