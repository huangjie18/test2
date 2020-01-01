#include"../jmesh_driver_config.h"
#if (JMESH_DRIVER_BX2400)
#include"FreeRTOS.h"
#include"semphr.h"
#include"../jmesh_uart.h"
#include"../../onchip_system/os_timer_event.h"
#include"stdio.h"
#include"app_uart.h"
#include"../jmesh_uart_callback.h"
#include"string.h"
#include"osapp_config.h"
#include"../../jmesh/jmesh_save_define.h"
#include"../../jmesh/jmesh_save.h"
#include"../../jmesh/jmesh_print.h"
#include"field_manipulate.h"
#include"dw_apb_uart_typedef.h"
#include"io_ctrl.h"
#include"log.h"
extern void uart_set_divisor(reg_uart_t *reg,uint16_t divisor);

#define UART_CACHE_BUFFER_SIZE 513
#define UART_RECV_INTERVAL_TIMEOUT_MS 100
static unsigned char uart0_buf;
static app_uart_inst_t uart0=UART_INSTANCE(0);
static app_uart_inst_t uart1=UART_INSTANCE(1);
typedef struct {
    unsigned char dat[UART_CACHE_BUFFER_SIZE];
    unsigned short len;
}common_dat_t;
baud_para_t baud_param;
static os_timer_event_t os_uart0_timer;
static os_timer_event_t os_uart1_timer;
static common_dat_t uart_data[1];
#ifndef x86
SemaphoreHandle_t  uart_xSemaphore=NULL;
#endif

static void jmesh_uart_rev_int_poll(void);

static void os_uart0_read_callback(void* buffer,unsigned char length);
static void os_uart1_read_callback(void* buffer,unsigned char length);

static void os_uart0_timer_handler(os_data_t data)
{
    if(uart_data[0].len)
	  {
		    jmesh_uart_recv_callback(JMESH_UART_0,uart_data[0].len,uart_data[0].dat);
        uart_data[0].len=0;
        memset(uart_data[0].dat,0x00,UART_CACHE_BUFFER_SIZE);
	  }
}

static void os_uart1_timer_handler(os_data_t data)
{
    if(uart_data[0].len!=0)
    {
        jmesh_uart_recv_callback(JMESH_UART_1,uart_data[0].len,uart_data[0].dat);
        memset(&uart_data[0],0,sizeof(common_dat_t));
        app_uart_read(&uart1.inst,uart_data[0].dat,1,os_uart1_read_callback,NULL);
    }
}

static void os_uart0_read_callback(void* dummy,unsigned char status)
{
    if(uart_data[0].len>=UART_CACHE_BUFFER_SIZE-1)
    {
        uart_data[0].len=0;
        memset(uart_data[0].dat,0x00,UART_CACHE_BUFFER_SIZE);
    }
    uart_data[0].dat[uart_data[0].len]=uart0_buf;
    uart_data[0].len++;
    app_uart_read(&uart0.inst,&uart0_buf,sizeof(uart0_buf),os_uart0_read_callback,NULL);
    jmesh_uart_rev_int_poll();
}

static void os_uart1_read_callback(void* buffer,unsigned char length)
{
    os_timer_event_restart(&os_uart1_timer);
    uart_data[0].len++;
    if(uart_data[0].len>=UART_CACHE_BUFFER_SIZE)
    {
        os_uart1_timer_handler(NULL);
        return;
    }
    app_uart_read(&uart1.inst,uart_data[0].dat+uart_data[0].len,1,os_uart1_read_callback,NULL);
}

void jmesh_uart0_send_callback(void)
{
#ifndef x86
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(uart_xSemaphore,&xHigherPriorityTaskWoken);
	if(xHigherPriorityTaskWoken == pdTRUE)
	{
		 portYIELD_FROM_ISR(pdTRUE);
	}
#endif
}
void wait_uart_send_over(void)
{
    #ifndef x86
	xSemaphoreTake(uart_xSemaphore,25);
    #endif
}
void jmesh_uart1_send_callback(void)
{

}
void jmesh_uart_init(JMESH_UART_NAME_T uart, JMESH_UART_BAUD_T baud, JMESH_UART_PARITY_T parity,JMESH_UART_STOP_BITS_T stop_bits){
    app_uart_inst_t *uart_inst;
    baud_para_t temp;
    unsigned char read_fail=0;
    uart_xSemaphore = xSemaphoreCreateBinary();
	  xSemaphoreGive(uart_xSemaphore );
    switch(uart)
    {
        case JMESH_UART_0:
            uart_inst=&uart0;
            uart_inst->param.rx_pin_no = 13;
		        io_pin_pull_write(13,IO_PULL_UP);
            uart_inst->param.tx_pin_no = 12;
            break;
        case JMESH_UART_1:
            uart_inst=&uart1;
            uart_inst->param.rx_pin_no = 21;
            uart_inst->param.tx_pin_no = 20;
            break;
        default:
            return;
    }
	  if(sizeof(baud_para_t)!=jmesh_save_read(JMESH_SAVE_BAUTRATE,sizeof(baud_para_t),(unsigned char*)&temp)){
	      read_fail=1;
	      temp.baud=baud;
	      temp.parity=parity;
	  }
    switch(temp.baud)
    {
        case JMESH_UART_BAUD_1200:
            uart_inst->param.baud_rate = UART_BAUDRATE_1200;
            break;
        case JMESH_UART_BAUD_2400:
            uart_inst->param.baud_rate = UART_BAUDRATE_2400;
            break;
        case JMESH_UART_BAUD_4800:
            uart_inst->param.baud_rate = UART_BAUDRATE_4800;
            break;
        case JMESH_UART_BAUD_9600:
            uart_inst->param.baud_rate = UART_BAUDRATE_9600;
            break;
        case JMESH_UART_BAUD_19200:
            uart_inst->param.baud_rate = UART_BAUDRATE_19200;
            break;
        case JMESH_UART_BAUD_38400:
            uart_inst->param.baud_rate = UART_BAUDRATE_38400;
            break;
        case JMESH_UART_BAUD_57600:
            uart_inst->param.baud_rate = UART_BAUDRATE_57600;
            break;
        case JMESH_UART_BAUD_115200:
            uart_inst->param.baud_rate = UART_BAUDRATE_115200;
            break;
        default:
            return;
    }
    switch(temp.parity)
    {
        case JMESH_UART_PARITY_EVEN:
            uart_inst->param.parity_en = 1;
			      uart_inst->param.even_parity = 1;
            break;
		    case JMESH_UART_PARITY_ODD :
            uart_inst->param.parity_en = 1;
			      uart_inst->param.even_parity = 0;
			      break;
        case JMESH_UART_PARITY_NONE:
            uart_inst->param.parity_en = 0;
            break;
        default:
            return;
    }
    if(read_fail){
        jmesh_save_write(JMESH_SAVE_BAUTRATE,sizeof(baud_para_t),(unsigned char*)&temp);
    }
    uart_inst->param.flow_control_en = 0;
    uart_inst->param.auto_flow_control = 0;
    uart_inst->param.tx_dma = 1;
    uart_inst->param.rx_dma = 1;
    app_uart_init(&uart_inst->inst);
	  jmesh_uart_start(uart);
}


void jmesh_uart_start(JMESH_UART_NAME_T uart){
    switch(uart)
    {
        case JMESH_UART_0:
            uart_data[0].len=0;
            memset(uart_data[0].dat,0x00,UART_CACHE_BUFFER_SIZE);
            app_uart_read(&uart0.inst,&uart0_buf,sizeof(uart0_buf),os_uart0_read_callback,NULL);
            os_timer_event_set(&os_uart0_timer,UART_RECV_INTERVAL_TIMEOUT_MS ,os_uart0_timer_handler,NULL);
            break;
        case JMESH_UART_1:
            app_uart_read(&uart1.inst,uart_data[0].dat,1,os_uart1_read_callback,NULL);
            os_timer_event_set(&os_uart1_timer,UART_RECV_INTERVAL_TIMEOUT_MS,os_uart1_timer_handler,NULL);
            break;
        default:
            return;
    }
}

void jmesh_uart_stop(JMESH_UART_NAME_T uart){
    unsigned int remain_len=0;
    unsigned int uart_read_size=0;
    switch(uart)
    {
        case JMESH_UART_0:
            wait_uart_send_over();
            app_uart_read_cancel(&uart0.inst,&remain_len,(void **)&uart_read_size);
            //app_uart_uninit(&uart0.inst);
            break;
        case JMESH_UART_1:
            app_uart_uninit(&uart1.inst);
            break;
        default:
            return;
    }
}

void jmesh_uart_send_no_take_semphr(JMESH_UART_NAME_T uart, unsigned short length, unsigned char *data){
    switch(uart)
    {
        case JMESH_UART_0:
            app_uart_write(&uart0.inst,data,length,(void (*) (void*,uint8_t))jmesh_uart0_send_callback,NULL);
            break;
        case JMESH_UART_1:
            app_uart_write(&uart1.inst,data,length,(void (*) (void*,uint8_t))jmesh_uart1_send_callback,NULL);
            break;
		    case JMESH_RTT_LOG :
			      //rtt_output(LOG_LVL_INFO,(const char*)data);
			      //LOG_RAW((const char*)data);
			      break;
        default:
            return;
    }
}

void jmesh_uart_send(JMESH_UART_NAME_T uart, unsigned short length, unsigned char *data){
    switch(uart)
    {
        case JMESH_UART_0:
			      wait_uart_send_over();
            app_uart_write(&uart0.inst,data,length,(void (*) (void*,uint8_t))jmesh_uart0_send_callback,NULL);
            break;
        case JMESH_UART_1:
            app_uart_write(&uart1.inst,data,length,(void (*) (void*,uint8_t))jmesh_uart1_send_callback,NULL);
            break;
		    case JMESH_RTT_LOG :
			      //rtt_log(LOG_LVL_INFO,(const char*)data);
			      //LOG_RAW((const char*)data);
			      break;
        default:
            return;
    }
}

signed char baudrate_to_enum(unsigned long baudrate)
{
	switch(baudrate)
	{
		case 1200 :
			return JMESH_UART_BAUD_1200;
		case 2400 :
			return JMESH_UART_BAUD_2400;
		case 4800 :
			return JMESH_UART_BAUD_4800;
		case 9600 :
			return JMESH_UART_BAUD_9600;
		case 19200 :
			return JMESH_UART_BAUD_19200;
		case 38400 :
			return JMESH_UART_BAUD_38400;
		case 57600:
			return JMESH_UART_BAUD_57600;
		case 115200 :
			return JMESH_UART_BAUD_115200;
		default :
			return -1;
	}
}
int jmesh_uart_baudrate_set(unsigned char enum_baud,unsigned char parity)
{
	unsigned char parity_en,even_parity;
	baud_para_t baud_param;
	switch(parity)
	{
		case JMESH_UART_PARITY_EVEN :
			parity_en = 1;
			even_parity = 1;
			//uart_pen_setf(1);
			//uart_eps_setf(1);
			break;
		case JMESH_UART_PARITY_ODD :
			parity_en = 1;
			even_parity = 0;
			//uart_pen_setf(1);
			//uart_eps_setf(0);
			break;
		case JMESH_UART_PARITY_NONE :
			parity_en = 0;
			even_parity = 0;
			//uart_pen_setf(0);
			break;
		default :
			return -1;
	}
  uart0.reg->LCR = FIELD_BUILD(UART_STOP, One_Stop_bit) | FIELD_BUILD(UART_DLS, Data_Length_8_bits)
			| FIELD_BUILD(UART_PEN, parity_en) | FIELD_BUILD(UART_EPS, even_parity);
	switch(enum_baud)
	{
		case JMESH_UART_BAUD_1200 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_1200&0x3ff);
			break;
		case JMESH_UART_BAUD_2400 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_2400&0x3ff);
			break;
		case JMESH_UART_BAUD_4800 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_4800&0x3ff);
			break;
		case JMESH_UART_BAUD_9600 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_9600&0x3ff);
			break;
		case JMESH_UART_BAUD_19200 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_19200&0x3ff);
			break;
		case JMESH_UART_BAUD_38400 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_38400&0x3ff);
			break;
		case JMESH_UART_BAUD_57600 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_38400&0x3ff);
			break;
		case JMESH_UART_BAUD_115200 :
			uart_set_divisor(uart0.reg,(uint16_t)UART_BAUDRATE_115200&0x3ff);
			break;
		default :
			return -1;
	}
	baud_param.baud = enum_baud;
	baud_param.parity = parity;
	//jmesh_save_write(JMESH_SAVE_BAUTRATE,sizeof(baud_para_t),(unsigned char*)&baud_param);
	return enum_baud;
}
signed long enum_to_baudrate(JMESH_UART_BAUD_T baud_enum)
{
	switch(baud_enum)
	{
		case JMESH_UART_BAUD_1200:
			return 1200;
		case JMESH_UART_BAUD_2400 :
			return 2400;
		case JMESH_UART_BAUD_4800 :
			return 4800;
		case JMESH_UART_BAUD_9600 :
			return 9600;
		case JMESH_UART_BAUD_19200 :
			return 19200;
		case JMESH_UART_BAUD_38400 :
			return 38400;
		case JMESH_UART_BAUD_57600:
			return 57600;
		case JMESH_UART_BAUD_115200 :
			return 115200;
		default :
			return -1    ;
	}
}

static volatile unsigned char uart_rev_int=0;

OS_TASK(os_uart_int_e,event,data)
{
    OS_START();
    while(1){
        OS_YIELD();
        if(uart_rev_int){
            uart_rev_int=0;
            os_timer_event_restart(&os_uart0_timer);
        }
    }
    OS_END();
}

static void jmesh_uart_rev_int_poll(void)
{
    uart_rev_int=1;
    os_task_poll(&os_uart_int_e,1);
}
#endif
