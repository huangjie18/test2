/*! @file si4438_hal.c
 * @brief 
 *
 */

#include "si4438_hal.h"
#include "jmesh_gpio.h"


void radio_port_init(void)
{
	// SND ������� 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_SDN);	
	
	// nSEL ������� 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_nSEL);	
	
	// SCLK ������� 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_SCLK);	
	
	// SDI ������� 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_SDI);
	
	// SDO �������� 
	jmesh_gpio_mode_in_pu(GPAIO_SI4438_SDO);	
	
	// nirq �������� 
	jmesh_gpio_mode_in_pu(GPAIO_SI4438_nIRQ);

	Clr_SI4438_SDN(); 
	Set_SI4438_nSEL();	
	Clr_SI4438_SCLK();
	Clr_SI4438_SDI();
}

	
