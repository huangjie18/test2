#include "periph_semaphore.h"
#include "ll.h"
#include "compiler_flag.h"

N_XIP_SECTION void periph_semaphore_init(periph_semaphore_t *periph_sem,uint8_t count)
{
    *periph_sem = count;
}

N_XIP_SECTION bool periph_semaphore_p(periph_semaphore_t *periph_sem)
{
    bool retval = false;
    ATOMIC_OP(
        if(*periph_sem > 0)
        {
            *periph_sem -= 1;
            retval = true;
        }
    );
    return retval;
}

N_XIP_SECTION void periph_semaphore_v(periph_semaphore_t *periph_sem)
{
    ATOMIC_OP(
        *periph_sem += 1;
    );
}

N_XIP_SECTION uint8_t periph_semaphore_get_count(periph_semaphore_t *periph_sem)
{
    return *periph_sem;
}
