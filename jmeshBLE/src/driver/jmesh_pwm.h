#ifndef JMESH_PWM_H
#define JMESH_PWM_H


typedef enum {
    JMESH_PWM_CHANNEL_0,
    JMESH_PWM_CHANNEL_1,
    JMESH_PWM_CHANNEL_2,
    JMESH_PWM_CHANNEL_3,
    JMESH_PWM_CHANNEL_4,
}jmesh_pwm_channel_t;

typedef enum {
	PIN_NUM_0,
	PIN_NUM_1,
    PIN_NUM_2,
	PIN_NUM_3,
	PIN_NUM_4,
	PIN_NUM_5,
}jmesh_pwm_pin_num_t;

extern void jmesh_pwm_start(jmesh_pwm_pin_num_t pin_num,jmesh_pwm_channel_t channel, unsigned short high_ms, unsigned short low_ms);
extern void jmesh_pwm_stop(jmesh_pwm_channel_t channel);
extern int jmesh_pwm_list(unsigned char *pin_num,jmesh_pwm_channel_t index, unsigned short *high_ms, unsigned short *low_ms,unsigned short *channel);


#endif // JMESH_PWM_H
