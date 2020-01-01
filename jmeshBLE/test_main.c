#include <stdio.h>
#include <stdlib.h>
#include"windows.h"
#include"src/driver/jmesh_uart.h"
#include"src/onchip_system/os_main.h"
#include"src/jmesh/jmesh_pdu.h"
#include"src/bearer/adv/jmesh_adv.h"
#include"src/bearer/beacon/jmesh_beacon.h"
#include"src/onchip_system/os_main.h"
#include"src/jmesh/jmesh_print.h"

void print_test(void)
{
    unsigned char tb[5]={1,2,3,4,5};
    JMESH_LOGE(35,"test","a test error string\n");
    JMESH_LOGW(35,"test","a test warn string\n");
    JMESH_LOGN("test","a test note string\n");
    JMESH_LOGI("test","a test info string\n");
    JMESH_LOGD("test","a test debug string\n");

    JMESH_LOGE_BUFFER(35,"test",5,tb,"a test error buffer:");
    JMESH_LOGW_BUFFER(35,"test",5,tb,"a test warn buffer:");
    JMESH_LOGN_BUFFER("test",5,tb,"a test note buffer:");
    JMESH_LOGI_BUFFER("test",5,tb,"a test info buffer:");
    JMESH_LOGD_BUFFER("test",5,tb,"a test debug buffer:");
}

int main()
{
    MSG msg;
    print_test();

    os_main();

    while(GetMessage(&msg,NULL,0,NULL)){
        if(msg.message== WM_TIMER){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}
