#ifndef WATER_GATE_H
#define WATER_GATE_H

#define WATER_NODE_SIZE 20
#define WATER_NODE_RECORD_SIZE 100

typedef struct st_water_gate_param{
    unsigned short record_interval_min;
    unsigned char recv_interval_min;
    unsigned char recv_window_sec;
}water_gate_param_t;
typedef struct st_water_node{
    unsigned short addr;
    unsigned char mac[7];
}water_node_t;
typedef struct st_water_record{
    unsigned long value;
    unsigned char unit;
    unsigned char status;
}water_record_t;

void water_gate_init(void);
int water_gate_list_node(unsigned char length,unsigned char* buffer);

water_record_t *water_gate_get_by_mac(unsigned char* mac);
water_record_t *water_gate_get_by_addr(unsigned short addr);

int water_gate_188(unsigned char in_len,unsigned char out_len,unsigned char* in,unsigned char* out);
int water_gate_645(unsigned char in_len,unsigned char out_len,unsigned char* in,unsigned char* out);

void water_gate_set_param(water_gate_param_t* param);
void water_gate_get_param(water_gate_param_t* param);
void water_gate_set_time(unsigned long second);
unsigned long water_gate_get_time(void);
water_record_t *water_gate_get_history(unsigned short addr,unsigned long time);
int water_gate_list_history(unsigned short addr,unsigned long start_time,unsigned char length,unsigned char* buffer);
void water_gate_report_recv(unsigned char* mac,unsigned short src,unsigned long value,unsigned char unit,unsigned char status);

int water_gate_188_handler(unsigned char interface,unsigned short len,unsigned char *data);
#endif // WATER_GATE_H
