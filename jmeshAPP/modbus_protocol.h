#ifndef MODBUS_PROTOCOL_H
#define MODBUS_PROTOCOL_H

#include <stdint.h> 
#include <stdbool.h> 

bool is_modbus_frame(uint8_t *data, uint8_t len);

#endif
