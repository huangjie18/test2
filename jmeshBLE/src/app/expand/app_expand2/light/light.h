#ifndef LIGHT_H
#define LIGHT_H


#define LIGHT_VALUE_NUM 5/**< red,yellow,green,white, */

void light_set(unsigned char* value);
void light_get(unsigned char* value);

void light_on(void);
void light_off(void);

void light_start_mode(unsigned short base_ticks,unsigned short on_ticks,unsigned short off_ticks,unsigned short live_ticks);


#endif // LIGHT_H
