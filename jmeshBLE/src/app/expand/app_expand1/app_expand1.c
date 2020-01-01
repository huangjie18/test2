#include"app_expand1.h"
#include"../app_expand.h"
#include"../../../jmesh/jmesh_types.h"
#include"app_expand1_device_client.h"
#include"app_expand1_device_server.h"


void jmesh_app_expand1_init(void)
{
    #ifdef JMESH_APP_EXPAND1_DEVICE_CLIENT
    jmesh_app_expand1_device_client_init();
    #endif

    #ifdef JMESH_APP_EXPAND1_DEVICE_CLIENT
    jmesh_app_expand1_device_server_init();
    #endif
}

