#ifndef KEHUA_TRANSFER_ARM_H
#define KEHUA_TRANSFER_ARM_H

extern unsigned char arm_state;

void arm_control_init(void);
void arm_control_on(void);
void arm_control_off(void);
void arm_control_stop(void);
unsigned char transfer_arm_rev_data_check(unsigned char *data,unsigned short len);
void stop_timer_handler(void* arg);
void report_kehua_arm_start(void);
unsigned char APP_BD_B101_ARM_handler(unsigned char *data,unsigned short len,unsigned char rev_type);
#endif
