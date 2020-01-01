#ifndef WATER_H
#define WATER_H




#define WATER_HISTORY_SIZE 10
#define WATER_RECORD_SIZE 3

#define WATER_MAC_SIZE 7

enum{
    WATER_TYPE_PULSE1,
    WATER_TYPE_PULSE2,
    WATER_TYPE_PULSE3,
    WATER_TYPE_NONMAGNETIC,
    WATER_TYPE_PHOTOELECTRICITY,
    WATER_TYPE_188,
};
typedef struct st_water_param{
    unsigned char type;
    unsigned char valve_pin;
    unsigned short report_intv_s;
    unsigned short report_dst;
    //unsigned short save_intv_s;
    unsigned char unit;
    unsigned char pulse_a;
    unsigned char pulse_b;
    unsigned char pulse_c;
    unsigned char mac[8];
}water_param_t;

void water_init(void);
void water_uninit(void);
void water_auto_report(void);
void water_set_mac(unsigned char* mac);
void water_get_mac(unsigned char* mac);
int water_set_value(unsigned long litre,unsigned char unit);
unsigned long water_get_value(void);
void water_save_value(void);
/*
    D0 D1   D2  D3  D4  D5  D6  D7
    00:开    0：正常
    01：关   1：欠压
    11：异常
*/

unsigned char water_get_unit(void);
unsigned char water_get_status(void);
int water_set_time(unsigned long second);
unsigned long water_get_time(void);
int water_get_history_data(unsigned long start_second,unsigned long end_second,unsigned short intv_s);
int water_get_history_record(void);
int water_set_valve(unsigned char onOff);
void water_set_type(unsigned char water_type);
unsigned char water_get_type(void);
void water_set_valve_pin(unsigned char valve_pin);
unsigned char water_get_valve_pin(void);
int water_set_report(unsigned short intv_s,unsigned short dst);
void water_get_report(unsigned short *intv_s,unsigned short *dst);
void water_set_value_unit(unsigned char value_unit);
unsigned char water_get_value_unit(void);
void water_set_pulse_pin(unsigned char pulse_a,unsigned char pulse_b,unsigned char pulse_c);
void water_get_pulse_pin(unsigned char *pulse_a,unsigned char *pulse_b,unsigned char *pulse_c);

void water_set_param(water_param_t* param);
void water_get_param(water_param_t* param);

void water_pulse_add(void);

#endif // WATER_H
