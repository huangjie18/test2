
#include"../jmesh_driver_config.h"
#if(JMESH_DRIVER_X86)

#include"../jmesh_uart.h"
#include"stdio.h"

void jmesh_uart_init(JMESH_UART_NAME_T uart, JMESH_UART_BAUD_T baud, JMESH_UART_PARITY_T parity,JMESH_UART_STOP_BITS_T stop_bits){
     ;
 }

void jmesh_uart_start(JMESH_UART_NAME_T uart){

}

void jmesh_uart_stop(JMESH_UART_NAME_T uart){
}

void jmesh_uart_send(JMESH_UART_NAME_T uart, unsigned short length, unsigned char *data){
    int i;
    for(i=0;i<length;i++){
        printf("%c",data[i]);
    }
}

int jmesh_uart_baudrate_set(unsigned char enum_baud,unsigned char parity)
{
    return 0;
}
void jmesh_uart_send_no_take_semphr(JMESH_UART_NAME_T uart, unsigned short length, unsigned char *data)
{
    jmesh_uart_send(uart,length,data);
}
signed char baudrate_to_enum(unsigned long baudrate)
{
    return 0;
}
int enum_to_baudrate(unsigned char baud_enum)
{

    return 0;
}
#endif

