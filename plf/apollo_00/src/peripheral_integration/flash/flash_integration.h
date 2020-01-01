#ifndef FLASH_INTEGRATION_H_
#define FLASH_INTEGRATION_H_
#include <stdint.h>
#include <stdbool.h>
#include "app_qspi.h"
#include "periph_error.h"

periph_err_t flash_program_execution(uint8_t cmd,uint32_t offset, uint32_t length, uint8_t *buffer);

periph_err_t flash_program_operation(uint8_t cmd,uint32_t offset, uint32_t length, uint8_t *buffer);

periph_err_t flash_erase_execution(uint8_t cmd,uint32_t addr,bool whole_chip);

periph_err_t flash_erase_operation(uint8_t cmd,uint32_t addr,bool whole_chip);

periph_err_t flash_read_execution(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len);

periph_err_t flash_read_operation(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len);

periph_err_t flash_multi_read_32bits_execution(qspi_multi_read_32bits_param_t *param);

periph_err_t flash_multi_read_32bits_operation(qspi_multi_read_32bits_param_t *param);

#endif

