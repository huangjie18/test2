#ifndef JMESH_APP_EXPAND2_REALCOMM_H
#define JMESH_APP_EXPAND2_REALCOMM_H

typedef enum{
    JMESH_APP_EXPAND2_REALCOMM_UART0=0,
    JMESH_APP_EXPAND2_REALCOMM_UART1=1,
    JMESH_APP_EXPAND2_REALCOMM_SPI=2,
    JMESH_APP_EXPAND2_REALCOMM_PORT_SIZE=3,
}JMESH_APP_EXPAND2_REALCOMM_PORT_T;

typedef enum{
    JMESH_APP_EXPAND2_REALCOMM_BAUD2400=0,
    JMESH_APP_EXPAND2_REALCOMM_BAUD4800=1,
    JMESH_APP_EXPAND2_REALCOMM_BAUD9600=2,
    JMESH_APP_EXPAND2_REALCOMM_BAUD19200=3,
    JMESH_APP_EXPAND2_REALCOMM_BAUD38400=4,
    JMESH_APP_EXPAND2_REALCOMM_BAUD115200=5,
}JMESH_APP_EXPAND2_REALCOMM_BAUDRATE_T;

typedef enum{
    JMESH_APP_EXPAND2_REALCOMM_PARITY_ZERO=0,
    JMESH_APP_EXPAND2_REALCOMM_PARITY_ODD=1,
    JMESH_APP_EXPAND2_REALCOMM_PARITY_EVEN=2,
    JMESH_APP_EXPAND2_REALCOMM_PARITY_NONE=3,

}JMESH_APP_EXPAND2_REALCOMM_PARITY_T;

int jmesh_app_expand2_realcomm_set(unsigned char port,unsigned char baud,unsigned char parity);
void jmesh_app_expand2_realcomm_trans(unsigned char port,unsigned short length,unsigned char* data);
void jmesh_app_expand2_realcomm_trans2(unsigned char port,unsigned short length,unsigned char* data);
void jmesh_app_expand2_realcomm_send(unsigned char port,unsigned short length,unsigned char* data);



#endif // JMESH_APP_EXPAND2_REALCOMM_H
