#ifndef PERIPH_MNGT_H_
#define PERIPH_MNGT_H_
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "periph_recovery.h"
typedef struct
{
    bool uart0_tx;
    bool uart0_rx;
    bool uart1_tx;
    bool uart1_rx;
    bool spim0;
    bool spim1;
    bool spis;
    bool iic0;
    bool iic1;
    bool pwm;
}periph_domain_stat_t;


typedef struct
{
    bool dmac;
    bool timer0;
    bool timer1;
    bool hwecc;
    bool adc_ctrl;
}cpu_domain_stat_t;


extern periph_domain_stat_t periph_domain_stat;
extern cpu_domain_stat_t cpu_domain_stat;
extern periph_recovery_mngt_t periph_domain_recovery_mngt;
extern periph_recovery_mngt_t cpu_domain_recovery_mngt;

bool periph_domain_busy(void);
bool cpu_domain_busy(void);

#endif
