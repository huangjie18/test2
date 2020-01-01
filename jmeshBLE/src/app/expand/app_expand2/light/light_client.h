#ifndef LIGHT_CLIENT_H
#define LIGHT_CLIENT_H


void light_client_init(void);
void light_client_set(unsigned short dst,unsigned char* value);
void light_client_get(unsigned short dst);
void light_client_status_callback(unsigned short src,unsigned char* value);

void light_client_mode_set(unsigned short dst,unsigned char id,unsigned short speed_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks,unsigned char direction);
void light_client_mode_get(unsigned short dst,unsigned char id);
void light_client_mode_callback(unsigned short src,unsigned char id,unsigned short speed_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks,unsigned char direction);

void light_client_mode(unsigned short dst,unsigned char id);

void light_client_bit_set(unsigned short dst,unsigned char length,unsigned char *value);

void light_client_point_set(unsigned short dst,unsigned char length,unsigned char *value);
#endif // LIGHT_CLIENT_H
