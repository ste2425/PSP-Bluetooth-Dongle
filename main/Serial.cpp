#include "Serial.h"

//#define SERIAL_DEBUG

void SerialWrapper_begin(unsigned long baud, uint32_t config, int rxPin, int txPin) {
    #ifdef SERIAL_DEBUG
        Serial.begin(baud);
    #else
        Serial2.begin(baud, config, rxPin, txPin);
    #endif
}

int SerialWrapper_read(void) {
    #ifdef SERIAL_DEBUG
        return Serial.read();
    #else
        return Serial2.read();
    #endif
}

size_t SerialWrapper_readBytes(char* buffer, size_t length) {
    #ifdef SERIAL_DEBUG
        return Serial.readBytes(buffer, length);
    #else
        return Serial2.readBytes(buffer, length);
    #endif
}

int SerialWrapper_available() {
    #ifdef SERIAL_DEBUG
        return Serial.available();
    #else
        return Serial2.available();
    #endif
}

void SerialWrapper_write(const uint8_t* buffer, size_t size) {
    #ifdef SERIAL_DEBUG
        Serial.write(buffer, size);
    #else
        Serial2.write(buffer, size);
    #endif
}

void SerialWrapper_write(uint8_t value) {
    #ifdef SERIAL_DEBUG
        Serial.write(value);
    #else
        Serial2.write(value);
    #endif
}

void SerialWrapper_flush() {
    #ifdef SERIAL_DEBUG
        Serial.flush();
    #else
        Serial2.flush();
    #endif
}
