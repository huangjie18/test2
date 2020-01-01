#ifndef SVCALL_H_
#define SVCALL_H_
#include <stdint.h>
#include "compiler.h"
enum svcall_num_enum
{
    SVCALL_FLASH_PROGRAM,
    SVCALL_FLASH_ERASE,
    SVCALL_FLASH_READ,
    SVCALL_FLASH_MULTI_READ_32BITS,
    SVCALL_NUM_MAX,
};



#endif
