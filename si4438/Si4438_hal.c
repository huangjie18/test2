/*! @file si4438_hal.c
 * @brief 
 *
 */

#include "si4438_hal.h"
#include "jmesh_gpio.h"


void radio_port_init(void)
{
	// SND 推挽输出 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_SDN);	
	
	// nSEL 推挽输出 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_nSEL);	
	
	// SCLK 推挽输出 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_SCLK);	
	
	// SDI 推挽输出 
	jmesh_gpio_mode_out_pp(GPAIO_SI4438_SDI);
	
	// SDO 上拉输入 
	jmesh_gpio_mode_in_pu(GPAIO_SI4438_SDO);	
	
	// nirq 上拉输入 
	jmesh_gpio_mode_in_pu(GPAIO_SI4438_nIRQ);

	Clr_SI4438_SDN(); 
	Set_SI4438_nSEL();	
	Clr_SI4438_SCLK();
	Clr_SI4438_SDI();
}

	
