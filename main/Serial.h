#pragma once

#include <Arduino.h>

void    SerialWrapper_begin(unsigned long baud, uint32_t config = SERIAL_8N1, int rxPin = -1, int txPin = -1);
size_t  SerialWrapper_readBytes(char* buffer, size_t length);
int     SerialWrapper_available();
void    SerialWrapper_write(const uint8_t* buffer, size_t size);
void    SerialWrapper_write(uint8_t value);
int     SerialWrapper_read(void);
void    SerialWrapper_flush();