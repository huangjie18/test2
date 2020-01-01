#ifndef WATER_PULSE_H
#define WATER_PULSE_H
#include"../../../../onchip_system/os_core.h"

#define WATER_PULSE_1_IO 16
#define WATER_PULSE_2_IO 22

#define WATER_PULSE_UNIT_LITRE 1
#define WATER_PULSE_MIN_INTERVAL_MS 1000
#define WATER_PULSE_IO_INVALID 0xff

OS_TASK_NAME(os_pulse_int_e);

void water_1pulse_uinit(unsigned char io_a);
void water_1pulse_init(unsigned char io_a);

void water_2pulse_uinit(unsigned char io_a,unsigned char io_b);
void water_2pulse_init(unsigned char io_a,unsigned char io_b);
void water_3pulse_uinit(unsigned char io_a,unsigned char io_b,unsigned char io_c);
void water_3pulse_init(unsigned char io_a,unsigned char io_b,unsigned char io_c);

#endif // WATER_PULSE_H
