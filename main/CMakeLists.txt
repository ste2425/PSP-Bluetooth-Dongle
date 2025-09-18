set(srcs
        "main.c"
        "sketch.cpp"
        "commandManager.cpp"
        "controllerManager.cpp"
        "Serial.cpp"
        "commands/controllerLED.cpp"
        "commands/newConnectons.cpp"
        "commands/disconnectControllers.cpp"
        "commands/ping.cpp"
        "commands/controllerData.cpp"
        "commands/forgetBluetoothKeys.cpp"
        "commands/controllerPlayerLED.cpp"
        "commands/controllerVibrate.cpp"
        "commands/controllerInfo.cpp")

set(requires "bluepad32" "bluepad32_arduino" "arduino" "btstack")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
