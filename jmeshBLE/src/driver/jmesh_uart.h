#ifndef JMESH_UART_H
#define JMESH_UART_H
#include"../onchip_system/os_core.h"
#define JMESH_UART_DEBUG JMESH_UART_0

#define JMESH_DEBUG_SEND(len,data)  \
    jmesh_uart_send_no_take_semphr(JMESH_UART_DEBUG, len, data)

typedef enum {
    JMESH_UART_0,
    JMESH_UART_1,
	  JMESH_RTT_LOG,
} JMESH_UART_NAME_T;
typedef enum {
	  JMESH_UART_BAUD_EMPTY,
    JMESH_UART_BAUD_1200,
    JMESH_UART_BAUD_2400,
    JMESH_UART_BAUD_4800,
    JMESH_UART_BAUD_9600,
    JMESH_UART_BAUD_19200,
    JMESH_UART_BAUD_38400,
    JMESH_UART_BAUD_57600,
	JMESH_UART_BAUD_115200,
	JMESH_UART_BAUD_MAX
} JMESH_UART_BAUD_T;
typedef enum {
    JMESH_UART_PARITY_NONE,
    JMESH_UART_PARITY_ODD,
    JMESH_UART_PARITY_EVEN,
	  JEMSH_UART_PARITY_MAX,
} JMESH_UART_PARITY_T;

typedef enum {
    JMESH_UART_STOP_BITS_1,
    JMESH_UART_STOP_BITS_2,
} JMESH_UART_STOP_BITS_T;
typedef struct{
    unsigned char baud;
    unsigned char parity;
}baud_para_t;
extern baud_para_t baud_param;

OS_TASK_NAME(os_uart_int_e);

extern void jmesh_uart_init(JMESH_UART_NAME_T uart, JMESH_UART_BAUD_T baud, JMESH_UART_PARITY_T parity,
                             JMESH_UART_STOP_BITS_T stop_bits);

extern void jmesh_uart_start(JMESH_UART_NAME_T uart);

extern void jmesh_uart_stop(JMESH_UART_NAME_T uart);

extern void jmesh_uart_send(JMESH_UART_NAME_T uart, unsigned short length, unsigned char *data);
void wait_uart_send_over(void);
void jmesh_uart_send_no_take_semphr(JMESH_UART_NAME_T uart, unsigned short length, unsigned char *data);
int jmesh_uart_baudrate_set(unsigned char enum_baud,unsigned char parity);
signed char baudrate_to_enum(unsigned long baudrate);
signed long enum_to_baudrate(JMESH_UART_BAUD_T baud_enum);

#endif // UART_H
