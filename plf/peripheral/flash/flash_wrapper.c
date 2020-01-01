#include <string.h>
#include "flash_integration.h"
#include "flash_wrapper.h"
#include "flash_base.h"
#include "arch_init.h"
#include "spi_flash_cmd.h"
#include "compiler_flag.h"
#include "app_dmac.h"


static periph_err_t flash_multi_read_word(uint32_t offset,uint16_t length,uint32_t *buffer,uint8_t type)
{
    qspi_multi_read_32bits_param_t param;
    param.data = buffer;
    param.addr_field.extend_byte = 0x00;
    param.addr_field.addr = offset;
    param.length = length;
    param.inst_l = Instruction_Length_8_bits;
    switch(type)
    {
        case Dual_Output_Read_Mode:
            param.cmd = FLASH_DUAL_OUTPUT_FAST_READ;
            param.trans_type = Both_Standard_SPI_Mode;
            param.wait_cycles = 0;
            param.dual_quad = Dual_SPI_Format;
        break;
        case Quad_Output_Read_Mode:
            param.cmd = FLASH_QUAD_OUTPUT_FAST_READ;
            param.trans_type = Both_Standard_SPI_Mode;
            param.wait_cycles = 0;
            param.dual_quad = Quad_SPI_Format;
        break;
        case Dual_IO_Read_Mode:
            param.cmd = FLASH_DUAL_IO_FAST_READ;
            param.trans_type = Instruction_Standard_Address_Specific;
            param.wait_cycles = 0;
            param.dual_quad = Dual_SPI_Format;
        break;
        case Quad_IO_Read_Mode:
            param.cmd = FLASH_QUAD_IO_FAST_READ;
            param.trans_type = Instruction_Standard_Address_Specific;
            param.wait_cycles = 4;
            param.dual_quad = Quad_SPI_Format;
        break;
    }
    return flash_multi_read_32bits_operation(&param);
}

static periph_err_t flash_multi_read(uint32_t offset,uint32_t length,uint8_t * buffer,uint8_t type)
{
    periph_err_t error = PERIPH_NO_ERROR;
    uint8_t head_bytes = (uint32_t)buffer % sizeof(uint32_t);
    if(head_bytes)
    {
        uint32_t head_word;
        error = flash_multi_read_word(offset,1,&head_word,type);
        if(error != PERIPH_NO_ERROR)
        {
            return PERIPH_NO_ERROR;
        }
        uint8_t head_length = length < (sizeof(uint32_t) -head_bytes) ? length : (sizeof(uint32_t) -head_bytes);
        memcpy(buffer,(uint8_t *)&head_word,head_length);
        buffer += head_length;
        offset += head_length;
        length -= head_length;
    }
    uint16_t max_transfer_size = dmac_get_max_block_transfer_size();
    uint32_t length_32bits = length/sizeof(uint32_t);
    while(length_32bits)
    {
        uint16_t transfer_size = length_32bits > max_transfer_size ? max_transfer_size : length_32bits;
        error = flash_multi_read_word(offset,transfer_size,(uint32_t *)buffer,type);
        if(error != PERIPH_NO_ERROR)
        {
            return error;
        }
        offset += transfer_size * sizeof(uint32_t);
        buffer += transfer_size * sizeof(uint32_t);
        length_32bits -= transfer_size;
    }
    uint8_t tail_bytes = length % sizeof(uint32_t);
    if(tail_bytes)
    {
        uint32_t tail_word;
        error = flash_multi_read_word(offset,1,&tail_word,type);
        if(error != PERIPH_NO_ERROR)
        {
            return PERIPH_NO_ERROR;
        }
        memcpy(buffer,&tail_word,tail_bytes);
    }
    return error;
}

periph_err_t flash_dual_output_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    return flash_multi_read(offset,length,buffer,Dual_Output_Read_Mode);
}

periph_err_t flash_quad_output_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    return flash_multi_read(offset,length,buffer,Quad_Output_Read_Mode);
}

periph_err_t flash_dual_io_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    return flash_multi_read(offset,length,buffer,Dual_IO_Read_Mode);
}

periph_err_t flash_quad_io_read(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    return flash_multi_read(offset,length,buffer,Quad_IO_Read_Mode);
}

periph_err_t (*flash_read_func_ptr_get(void))(uint32_t,uint32_t,uint8_t*)
{
    extern uint32_t flash_op_mode;
    uint8_t (*fn)(uint32_t,uint32_t,uint8_t*)= NULL;
    switch(flash_op_mode)
    {
    case Std_Read_Mode:
        fn= flash_read;
    break;
    case Dual_Output_Read_Mode:
        fn= flash_dual_output_read;
    break;
    case Quad_Output_Read_Mode:
        fn= flash_quad_output_read;
    break;
    case Dual_IO_Read_Mode:
        fn= flash_dual_io_read;
    break;
    case Quad_IO_Read_Mode:
        fn= flash_quad_io_read;
    break;
    }
    return fn;
}

periph_err_t flash_erase_security_reg(uint8_t reg_num)
{
    return flash_erase_operation(FLASH_ERASE_SECURITY_REGISTERS,reg_num<<12,false);
}

periph_err_t flash_program_security_reg(uint8_t reg_num,uint16_t offset,uint16_t length,uint8_t *buffer)
{
    return flash_program_operation(FLASH_PROGRAM_SECURITY_REGISTERS,reg_num<<12|offset&0x1ff,length,buffer);
}

periph_err_t flash_program(uint32_t offset, uint32_t length, uint8_t *buffer)
{
    periph_err_t error;
    uint32_t offset_base = offset&(~(FLASH_PAGE_SIZE-1));
    if(offset_base!=offset)
    {
        uint32_t tailing_length = offset_base+FLASH_PAGE_SIZE-offset;
        uint32_t prog_length = length<tailing_length? length : tailing_length;
        error = flash_program_operation(FLASH_PAGE_PROGRAM,offset,prog_length,buffer);
        if(error != PERIPH_NO_ERROR)
        {
            return error;
        }
        buffer += prog_length;
        offset += prog_length;
        length -= prog_length;
    }
    uint16_t cycles = length / FLASH_PAGE_SIZE;
    uint16_t remainder = length % FLASH_PAGE_SIZE;
    uint16_t i;
    for(i=0;i<cycles;++i)
    {
        error = flash_program_operation(FLASH_PAGE_PROGRAM,offset,FLASH_PAGE_SIZE,buffer);
        if(error != PERIPH_NO_ERROR)
        {
            return error;
        }
        offset += FLASH_PAGE_SIZE;
        buffer += FLASH_PAGE_SIZE;
    }
    if(remainder)
    {
        error = flash_program_operation(FLASH_PAGE_PROGRAM,offset,remainder,buffer);
    }
    return error;
}

periph_err_t flash_erase(uint32_t offset,erase_t type)
{
    uint8_t cmd;
    bool whole_chip;
    switch(type)
    {
    case Sector_Erase:
        cmd = FLASH_SECTOR_ERASE;
        whole_chip = false;
        break;
    case Block_32KB_Erase:
        cmd = FLASH_32KB_BLOCK_ERASE;
        whole_chip = false;
        break;
    case Block_64KB_Erase:
        cmd = FLASH_64KB_BLOCK_ERASE;
        whole_chip = false;
        break;
    case Chip_Erase:
        cmd = FLASH_CHIP_ERASE;
        whole_chip = true;
        break;
    }    
    return flash_erase_operation(cmd,offset,whole_chip);
}

periph_err_t flash_read_sfdp(uint32_t addr,uint8_t *buf,uint8_t buf_len)
{
    uint8_t flash_write_cmd[5] = {FLASH_READ_SFDP,(addr>>16)&0xff,(addr>>8)&0xff,addr&0xff,0};
    return flash_read_operation(flash_write_cmd,sizeof(flash_write_cmd), buf, buf_len);
}

periph_err_t flash_read_jedec_ptp(uint32_t *jedec_ptp)
{
    return flash_read_sfdp(0x0c,(uint8_t *)jedec_ptp,sizeof(uint32_t));
}

periph_err_t flash_read_manufacturer_device_id(uint8_t *manufacturer_id,uint8_t *device_id)
{
    uint8_t read_cmd[4] = {FLASH_READ_DEVICE_ID, 0, 0, 0};
    uint16_t data;
    periph_err_t error = flash_read_operation((uint8_t*)&read_cmd,sizeof(read_cmd), (uint8_t*)&data, sizeof(data));
    if(error == PERIPH_NO_ERROR)
    {
        *manufacturer_id = data&0xff;
        *device_id = data>>8 & 0xff;
    }
    return error;
}

periph_err_t flash_read_jedec_id(uint8_t *manufacturer_id,uint8_t *mem_type,uint8_t *capacity)
{
    uint8_t read_cmd = FLASH_READ_JEDEC_ID;
    uint8_t data[3];
    periph_err_t error = flash_read_operation(&read_cmd,sizeof(read_cmd),data,sizeof(data));
    if(error == PERIPH_NO_ERROR)
    {
        *manufacturer_id = data[0];
        *mem_type = data[1];
        *capacity = data[2];
    }
    return error;
}

periph_err_t flash_read_memory_density(uint32_t *mem_density)
{
    periph_err_t error;
    do{
        uint32_t jedec_ptp;
        error = flash_read_jedec_ptp(&jedec_ptp);
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
        error = flash_read_sfdp(jedec_ptp + 4,(uint8_t *)mem_density,sizeof(uint32_t));
    }while(0);
    return error;
}

periph_err_t flash_reset()
{
    periph_err_t error;
    do{
        uint8_t enable_reset = FLASH_ENABLE_RESET;
        error = flash_write_no_dma_operation_start(&enable_reset,sizeof(uint8_t));
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
        uint8_t reset_device = FLASH_RESET_DEVICE;
        error = flash_write_no_dma_operation_start(&reset_device,sizeof(uint8_t));
        if(error != PERIPH_NO_ERROR)
        {
            break;
        }
    }while(0);
    return error;
}

N_XIP_SECTION periph_err_t flash_wakeup()
{
    uint8_t flash_res_cmd = FLASH_RELEASE_POWER_DOWN;
    return flash_write_no_dma_operation_start(&flash_res_cmd,sizeof(flash_res_cmd));
}

N_XIP_SECTION periph_err_t flash_deep_power_down()
{
    uint8_t flash_dp_cmd = FLASH_DEEP_POWER_DOWN;
    return flash_write_no_dma_operation_start(&flash_dp_cmd, sizeof(flash_dp_cmd));
}

static periph_err_t flash_read_byte(uint32_t offset, uint16_t length, uint8_t *buffer)
{
    uint8_t read_cmd[4] = {FLASH_READ_DATA_BYTES,offset>>16&0xff,offset>>8&0xff,offset&0xff};
    return flash_read_operation(read_cmd,sizeof(read_cmd), buffer, length);
}

periph_err_t flash_read(uint32_t offset,uint32_t length,uint8_t *buffer)
{
    uint16_t max_transfer_size = dmac_get_max_block_transfer_size();
    uint32_t cycles = length / max_transfer_size;
    uint16_t remaining_bytes = length % max_transfer_size;
    periph_err_t error = PERIPH_NO_ERROR;
    while(cycles--)
    {
        error = flash_read_byte(offset,max_transfer_size,buffer);
        if(error != PERIPH_NO_ERROR)
        {
            return PERIPH_NO_ERROR;
        }
        offset += max_transfer_size;
        buffer += max_transfer_size;
    }
    if(remaining_bytes)
    {
        error = flash_read_byte(offset,remaining_bytes,buffer);
    }
    return error;
}

periph_err_t flash_read_security_reg(uint8_t reg_num,uint16_t offset,uint16_t length,uint8_t *buffer)
{
    uint8_t read_cmd[5] = {FLASH_READ_SECURITY_REGISTERS,0,reg_num<<4 | (offset>>8 & 0x1),offset&0xff,0};
    return flash_read_operation(read_cmd,sizeof(read_cmd), buffer, length);
}


//#define FLASH_TEST
#ifdef FLASH_TEST
#include "io_ctrl.h"
#include "log.h"
#include <stdlib.h>
#define SECTOR_SIZE 0x1000
#define FLASH_SIZE (1<<20)
#define PAGE_SIZE_IN_WORD (FLASH_PAGE_SIZE/sizeof(uint32_t))
#define SECTOR_SIZE_IN_WORD (SECTOR_SIZE/sizeof(uint32_t))
uint32_t fbuf[SECTOR_SIZE_IN_WORD];
uint32_t fsrc[SECTOR_SIZE_IN_WORD];
void flash_test()
{
//    sysc_cpu_qspi_en_setf(0xf);
//    *(uint32_t*)0x20201074 = 0x0010000;// 0x8010000 on V2/0x0010000 on V3 - set VDD_PAD3 to 3.3V for qspi
    uint32_t offset = 0;
    io_cfg_output(23);
    io_pin_clear(23);
    while(offset<FLASH_SIZE)
    {
        /* erase test */
        io_pin_set(23);
        flash_erase(offset,Sector_Erase);
        io_pin_clear(23);
        flash_quad_output_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        //flash_quad_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        uint32_t i;
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            if(fbuf[i] != 0xffffffff)
            {
                BX_ASSERT(0);
                break;
            }
        }
        /* program test */
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            fsrc[i] = rand();
        }
        io_pin_set(23);
        flash_program(offset,SECTOR_SIZE,(uint8_t *)fsrc);
        io_pin_clear(23);
        flash_quad_output_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        //flash_quad_read(offset,SECTOR_SIZE,(uint8_t *)fbuf);
        for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
        {
            if(fsrc[i]!=fbuf[i])
            {
            	LOG(LOG_LVL_ERROR,"%d\n",i);
                BX_ASSERT(0);
                break;
            }
        }
        offset += SECTOR_SIZE;
    }

//    __asm("bkpt 01");
}

void flash_security_reg_test()
{
    uint8_t reg_num = 1;
    while(reg_num<=3)
    {
        flash_erase_security_reg(reg_num);
        flash_read_security_reg(reg_num,0, FLASH_PAGE_SIZE,(uint8_t *)fbuf);
        uint32_t i;
        for(i=0;i<PAGE_SIZE_IN_WORD;++i)
        {
            if(fbuf[i] != 0xffffffff)
            {
                BX_ASSERT(0);
                break;
            }
        }
        for(i=0;i<PAGE_SIZE_IN_WORD;++i)
        {
            fsrc[i] = rand();
        }
        flash_program_security_reg(reg_num, 0, FLASH_PAGE_SIZE,(uint8_t *)fsrc);
        flash_read_security_reg(reg_num,0, FLASH_PAGE_SIZE, (uint8_t * )fbuf);
        for(i=0;i<PAGE_SIZE_IN_WORD;++i)
        {
            if(fsrc[i]!=fbuf[i])
            {
                LOG(LOG_LVL_ERROR,"%d\n",i);
                BX_ASSERT(0);
                break;
            }
        }
        ++reg_num;
    }
}
#endif
#if 0
#include "gpio.h"
void flash_readID_winbond(void)
{
    sysc_cpu_qspi_en_setf(0x3);
    struct gpioConfigStruct gpio_28_29;

    gpio_Init();

    memset(&gpio_28_29, 0, sizeof(gpio_28_29));

    gpio_InitN(gpio3_4);
    gpio_InitN(gpio3_5);

    // cfg3_5: output/high
    gpio_28_29.debounce = gpio_Deb;
    gpio_28_29.dir = gpio_Output;
    gpio_28_29.value = gpio_High;

    gpio_SetConfig(gpio3_4, &gpio_28_29);
    gpio_SetConfig(gpio3_5, &gpio_28_29);

    flash_quad_enable();
    sysc_cpu_qspi_en_setf(0xf);

    uint8_t write_en_cmd = FLASH_WRITE_ENABLE;
    qspi_write(&write_en_cmd,sizeof(write_en_cmd));

    uint32_t jedec_ID;
    uint8_t read_stat_cmd = 0x9f; // 0x9f = Read JEDEC ID
    qspi_read((uint8_t*)&jedec_ID,sizeof(jedec_ID)-1,&read_stat_cmd,sizeof(read_stat_cmd));
    flash_operation_wait();

    LOG(LOG_LVL_INFO,"jedec_ID=0x%x\n", jedec_ID);
}
#endif
