#ifndef JMESH_APP_EXPAND2_H
#define JMESH_APP_EXPAND2_H
#include"../app_expand.h"


enum{
    JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_SET=1,
    JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_STATUS=2,
    JMESH_APP_EXPAND2_REALCOMM_REPORT_SET=3,
    JMESH_APP_EXPAND2_REALCOMM_REPORT_STATUS=4,
    JMESH_APP_EXPAND2_TRANSPARENT_TRANS=5,
	JMESH_APP_EXPAND2_TRANSPARENT_TRANS_STATUS=6,
	JMESH_APP_EXPAND2_REALCOMM_TRANS=7,
    JMESH_APP_EXPAND2_REALCOMM_TRANS_STATUS=8,
    /**< user define */
    JMESH_APP_EXPAND2_LIGHT_SET=11,
    JMESH_APP_EXPAND2_LIGHT_GET=12,
    JMESH_APP_EXPAND2_LIGHT_STATUS=13,
    JMESH_APP_EXPAND2_LIGHT_MODE_SET=14,
    JMESH_APP_EXPAND2_LIGHT_MODE_GET=15,
    JMESH_APP_EXPAND2_LIGHT_MODE=16,
    JMESH_APP_EXPAND2_LIGHT_MODE_STATUS=17,
    JMESH_APP_EXPAND2_LIGHT_BIT=18,
    JMESH_APP_EXPAND2_LIGHT_POINT=19,
    JMESH_APP_EXPAND2_WATER_READ_VALUE=20,
    JMESH_APP_EXPAND2_WATER_WRITE_VALUE=21,
    JMESH_APP_EXPAND2_WATER_SET_VALVE=22,
    JMESH_APP_EXPAND2_WATER_VALUE_STATUS=23,
    JMESH_APP_EXPAND2_WATER_SET_PARAM=24,
    JMESH_APP_EXPAND2_WATER_READ_PARAM=25,
    JMESH_APP_EXPAND2_WATER_PARAM_STATUS=26,
    JMESH_APP_EXPAND2_WATER_SET_MAC=27,
    JMESH_APP_EXPAND2_WATER_READ_MAC=28,
    JMESH_APP_EXPAND2_WATER_MAC_STATUS=29,
    JMESH_APP_EXPAND2_WATER_REPORT=30,

    JMESH_APP_EXPAND2_WATER_GET_VALVE,
    JMESH_APP_EXPAND2_WATER_SET_TYPE,
    JMESH_APP_EXPAND2_WATER_GET_TYPE,
    JMESH_APP_EXPAND2_WATER_SET_VALVE_PIN,
    JMESH_APP_EXPAND2_WATER_GET_VALVE_PIN,
    JMESH_APP_EXPAND2_WATER_SET_REPORT_PARA,
    JMESH_APP_EXPAND2_WATER_GET_REPORT_PARA,
    JMESH_APP_EXPAND2_WATER_SET_VALUE_UNIT,
    JMESH_APP_EXPAND2_WATER_GET_VALUE_UNIT,
    JMESH_APP_EXPAND2_WATER_SET_PULSE_PIN,
    JMESH_APP_EXPAND2_WATER_GET_PULSE_PIN,

    JMESH_APP_EXPAND2_WATER_TYPE_STATUS,
    JMESH_APP_EXPAND2_VALVE_PIN_STATUS,
    JMESH_APP_EXPAND2_REPORT_PARA_STATUS,
    JMESH_APP_EXPAND2_VALUE_UNIT_STATUS,
    JMESH_APP_EXPAND2_PULSE_PIN_STATUS,
		
		/* app define*/
		expand_opcode_realcomm_data,
		expand_opcode_realcomm_data_return,
		expand_opcode_realcomm_get_meter_addr,
		expand_opcode_realcomm_get_meter_addr_return,
		expand_opcode_realcomm_meter_reprot,
		expand_opcode_realcomm_meter_reprot_return,
		expand_opcode_realcomm_app_read_meter,
		expand_opcode_realcomm_app_read_meter_return,
		expand_opcode_realcomm_app_network_maintain,
		expand_opcode_realcomm_app_network_maintain_return,
		expand_opcode_realcomm_app_broadcast_data,
		expand_opcode_realcomm_app_set_meter,
		expand_opcode_realcomm_app_set_meter_return,
		expand_opcode_realcomm_app_read_meter_by_node,
		expand_opcode_realcomm_app_read_meter_by_node_return,
};



void jmesh_app_expand2_init(void);
void jmesh_set_device_roles(jmesh_device_role_t device_roles);
jmesh_device_role_t jmesh_get_device_roles(void);

#endif // JMESH_APP_EXPAND1_H
