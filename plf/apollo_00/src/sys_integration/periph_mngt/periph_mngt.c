#include <stddef.h>
#include "periph_mngt.h"
#include "uart_integration.h"
#include "log.h"
#define PERIPH_DOMAIN_RECOVERY_BUF_SIZE 8
#define CPU_DOMAIN_RECOVERY_BUF_SIZE 10

periph_inst_handle_t periph_domain_recovery_buf[PERIPH_DOMAIN_RECOVERY_BUF_SIZE];
periph_recovery_mngt_t periph_domain_recovery_mngt = {
    .hdl = periph_domain_recovery_buf,
    .mask = 0,
    .buf_size = PERIPH_DOMAIN_RECOVERY_BUF_SIZE,
};
periph_inst_handle_t cpu_domain_recovery_buf[CPU_DOMAIN_RECOVERY_BUF_SIZE];
periph_recovery_mngt_t cpu_domain_recovery_mngt = {
    .hdl = cpu_domain_recovery_buf,
    .mask = 0,
    .buf_size = CPU_DOMAIN_RECOVERY_BUF_SIZE,
};

periph_domain_stat_t periph_domain_stat;
cpu_domain_stat_t cpu_domain_stat;

static bool is_all_zero(uint8_t *src,uint32_t size)
{
    while(size--)
    {
        if(src[size]) return false;
    }
    return true;
}

bool periph_domain_busy(void)
{
    return is_all_zero((uint8_t *)&periph_domain_stat, sizeof(periph_domain_stat)) ? false : true;
}

bool cpu_domain_busy(void)
{
    return is_all_zero((uint8_t *)&cpu_domain_stat,sizeof(cpu_domain_stat)) ? false : true;
}

