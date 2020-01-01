#ifndef WATER_CLIENT_H
#define WATER_CLIENT_H
#include"water.h"

void water_client_init(void);
void water_client_read(unsigned short dst);

void water_client_read_status_callback(unsigned short src,unsigned long value);
void water_client_write_status_callback(unsigned short src,unsigned char status,unsigned long value_before,unsigned long value_after);

void water_client_control(unsigned short dst,unsigned char onOff);
void water_client_control_status_callback(unsigned short src,unsigned char onOff);





void water_client_init(void);
void water_client_set_type(unsigned short dst,unsigned char water_type);
void water_client_get_type(unsigned short dst);
void water_client_set_valve_pin(unsigned short dst,unsigned char valve_pin);
void water_client_get_valve_pin(unsigned short dst);
void water_client_set_report_para(unsigned short dst,unsigned short report_intv_ms,unsigned short report_dst);
void water_client_get_report_para(unsigned short dst);
void water_client_set_value_unit(unsigned short dst,unsigned char value_unit);
void water_client_get_value_unit(unsigned short dst);
void water_client_set_pulse_pin(unsigned short dst,unsigned char pulse_a,unsigned char pulse_b,unsigned char pulse_c);
void water_client_get_pulse_pin(unsigned short dst);
void water_client_read_value(unsigned short dst);
void water_client_write_value(unsigned short dst,unsigned long value,unsigned char unit);
void water_client_set_valve(unsigned short dst,unsigned char status);
void water_client_get_valve(unsigned short dst);
void water_client_set_param(unsigned short dst,water_param_t *param);
void water_client_read_param(unsigned short dst);
void water_client_set_mac(unsigned short dst,unsigned char* mac);
void water_client_read_mac(unsigned short dst);

void water_client_value_status_callback(unsigned short src,unsigned long value,unsigned char unit,unsigned char status);
void water_client_param_status_callback(unsigned short src,water_param_t *param);
void water_client_mac_status_callback(unsigned short src,unsigned char* mac);

#endif // WATER_CLIENT_H
