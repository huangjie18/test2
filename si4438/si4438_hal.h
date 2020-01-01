//---------------------------------------------------
// file name : si4438_hal.h
//--------------------------------------------------- 
#ifndef SI4438_HAL_H
#define SI4438_HAL_H

#include "io_ctrl.h"

/*******************************
4438π‹Ω≈∫Í∂®“Â
********************************/
#define GPAIO_SI4438_SDN     17
#define GPAIO_SI4438_nSEL    2
#define GPAIO_SI4438_nIRQ    15
#define GPAIO_SI4438_SDI     6
#define GPAIO_SI4438_SDO     5
#define GPAIO_SI4438_SCLK    4


#define Set_SI4438_SDN()   io_pin_set(GPAIO_SI4438_SDN)
#define Clr_SI4438_SDN()   io_pin_clear(GPAIO_SI4438_SDN)
#define Set_SI4438_nSEL()  io_pin_set(GPAIO_SI4438_nSEL)
#define Clr_SI4438_nSEL()  io_pin_clear(GPAIO_SI4438_nSEL)
#define Set_SI4438_SDI()   io_pin_set(GPAIO_SI4438_SDI)
#define Clr_SI4438_SDI()   io_pin_clear(GPAIO_SI4438_SDI)
#define Set_SI4438_SCLK()  io_pin_set(GPAIO_SI4438_SCLK)
#define Clr_SI4438_SCLK()  io_pin_clear(GPAIO_SI4438_SCLK)


#define Read_SI4438_nIRQ() io_pin_read(GPAIO_SI4438_nIRQ)
#define Read_SI4438_SDO()  io_pin_read(GPAIO_SI4438_SDO)


void radio_port_init(void);
#endif	//SI4438_HAL_H
//---------------------------------------------------
// end of file
//---------------------------------------------------
