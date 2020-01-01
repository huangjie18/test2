#include "bx_config.h"
#include "apollo_00.h"
#include "app_qspi_wrapper.h"
#include "app_qspi.h"
#include "arch_init.h"
#include "compiler_flag.h"

#define QSPI_CS_SEL_MASK 0x1
app_qspi_inst_t qspi_inst = QSPI_INSTANCE();

N_XIP_SECTION void app_qspi_param_init_wrapper()
{
    extern uint32_t flash_op_mode;
    qspi_inst.param.clk_div = 2;
    qspi_inst.param.rxsample_dly = 1;
    switch(flash_op_mode)
    {
    case Std_Read_Mode:
        qspi_inst.param.mode = Standard_SPI_Format;
    break;
    case Dual_Output_Read_Mode:
    case Dual_IO_Read_Mode:
        qspi_inst.param.mode = Dual_SPI_Format;
    break;
    case Quad_Output_Read_Mode:
    case Quad_IO_Read_Mode:
        qspi_inst.param.mode = Quad_SPI_Format;
    break;
    }
    qspi_inst.param.cs_pin_no[0] = QSPI_CS0_PIN_NO;
    qspi_inst.param.clk_pin_no = QSPI_CLK_PIN_NO;
    qspi_inst.param.data_pin_no[0] = QSPI_D0_PIN_NO;
    qspi_inst.param.data_pin_no[1] = QSPI_D1_PIN_NO;
    qspi_inst.param.data_pin_no[2] = QSPI_D2_PIN_NO;
    qspi_inst.param.data_pin_no[3] = QSPI_D3_PIN_NO;
}

N_XIP_SECTION void app_qspi_init_wrapper()
{
    app_qspi_init(&qspi_inst.inst); 
}

N_XIP_SECTION periph_err_t app_qspi_std_write_wrapper(uint8_t *data,uint32_t length)
{
    periph_err_t error = app_qspi_std_write(&qspi_inst.inst,QSPI_CS_SEL_MASK,data,length);
    return error;
}

N_XIP_SECTION periph_err_t app_qspi_std_write_no_dma_wrapper(uint8_t *data,uint16_t length)
{
    return app_qspi_std_write_no_dma(&qspi_inst.inst,QSPI_CS_SEL_MASK,data,length);
}

N_XIP_SECTION periph_err_t app_qspi_flash_program_wrapper(uint8_t cmd,uint32_t addr,uint8_t *data,uint32_t length)
{
    periph_err_t error = app_qspi_flash_program(&qspi_inst.inst,QSPI_CS_SEL_MASK,cmd,addr,data,length);
    return error;
}

N_XIP_SECTION periph_err_t app_qspi_std_read_wrapper(uint8_t *cmd_buf,uint8_t cmd_len,uint8_t *data_buf,uint16_t data_len)
{
    periph_err_t error = app_qspi_std_read(&qspi_inst.inst,QSPI_CS_SEL_MASK,cmd_buf,cmd_len,data_buf,data_len);
    return error;
}

N_XIP_SECTION periph_err_t app_qspi_multi_read_32bits_wrapper(qspi_multi_read_32bits_param_t *param)
{
    periph_err_t error = app_qspi_multi_read_32bits(&qspi_inst.inst,QSPI_CS_SEL_MASK,param);
    return error;
}

N_XIP_SECTION periph_err_t app_qspi_config_for_cache_wrapper(qspi_multi_read_32bits_param_t *param)
{
    return app_qspi_config_for_cache(&qspi_inst.inst,QSPI_CS_SEL_MASK,param);
}

N_XIP_SECTION uint32_t app_qspi_dr_reg_addr_for_cache_wrapper()
{
    return app_qspi_dr_reg_addr_for_cache(&qspi_inst.inst);
}

N_XIP_SECTION periph_err_t app_qspi_disable_for_cache_wrapper()
{
    return app_qspi_disable_for_cache(&qspi_inst.inst);
}

