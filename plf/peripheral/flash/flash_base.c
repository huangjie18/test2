#include <stdint.h>        
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bx_dbg.h"
#include "spi_flash_cmd.h"
#include "log.h"
#include "flash_base.h"
#include "arch_init.h"
#include "app_qspi_wrapper.h"
#include "compiler_flag.h"

N_XIP_SECTION static periph_err_t flash_operation_wait(uint8_t *status_reg_1)
{
    uint8_t flash_status,read_stat_cmd = FLASH_READ_STATUS_REGISTER_1;
    periph_err_t error;
    do
    {
        error = app_qspi_std_read_wrapper(&read_stat_cmd,sizeof(uint8_t),&flash_status,sizeof(uint8_t));
    }while((flash_status&0x1) && error == PERIPH_NO_ERROR);                 // wait until erase done
    if(error == PERIPH_NO_ERROR)
    {
        if(status_reg_1)
        {
            *status_reg_1 = flash_status;
        }
    }
    return error;
}

N_XIP_SECTION static periph_err_t flash_write_enable()
{
    uint8_t write_en_cmd = FLASH_WRITE_ENABLE;
    return app_qspi_std_write_wrapper(&write_en_cmd,sizeof(write_en_cmd));
}

N_XIP_SECTION periph_err_t flash_program_operation_start(uint8_t cmd,uint32_t offset, uint32_t length, uint8_t *buffer)
{
    periph_err_t error;
    do
    {
        error = flash_write_enable();
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
        error = app_qspi_flash_program_wrapper(cmd,offset,buffer,length);
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
        error = flash_operation_wait(NULL);
    }while(0);
    return error;
}

N_XIP_SECTION periph_err_t flash_erase_operation_start(uint8_t cmd,uint32_t addr,bool whole_chip)
{
    periph_err_t error;
    do
    {
        error = flash_write_enable();
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
        if(whole_chip)
        {
            error = app_qspi_std_write_wrapper(&cmd,sizeof(cmd));
        }else
        {
            uint8_t erase_cmd_addr[4];
            erase_cmd_addr[0] = cmd;
            erase_cmd_addr[1] = addr>>16 & 0xff;
            erase_cmd_addr[2] = addr>>8 & 0xff;
            erase_cmd_addr[3] = addr & 0xff;
            error = app_qspi_std_write_wrapper(erase_cmd_addr,sizeof(erase_cmd_addr));
        }
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
        error = flash_operation_wait(NULL);
    }while (0);
    return error;
}


N_XIP_SECTION periph_err_t flash_multi_read_32bits_operation_start(qspi_multi_read_32bits_param_t *param)
{
    return app_qspi_multi_read_32bits_wrapper(param);
}

N_XIP_SECTION periph_err_t flash_read_operation_start(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len)
{
    return app_qspi_std_read_wrapper(cmd_buf,cmd_len,data_buf,data_len);
}

N_XIP_SECTION periph_err_t flash_write_operation_start(uint8_t *data,uint32_t length)
{
    return app_qspi_std_write_wrapper(data,length);
}

N_XIP_SECTION periph_err_t flash_write_no_dma_operation_start(uint8_t *data,uint16_t length)
{
    return app_qspi_std_write_no_dma_wrapper(data,length);
}

