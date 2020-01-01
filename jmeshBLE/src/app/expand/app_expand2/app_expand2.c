#include"app_expand2.h"
#include"app_expand2_realcomm_client.h"
#include"app_expand2_realcomm_server.h"

#include"../app_expand.h"
#include"../../../jmesh/jmesh_types.h"

#include"../water/water.h"
#include"../water/water_client.h"
#include"../water/water_server.h"
#include"../water/water_gate.h"
#include"../../../jmesh/jmesh_save.h"
static jmesh_device_role_t jmesh_device_roles;
void jmesh_device_roles_init(void)
{
    if(sizeof(jmesh_device_role_t)!=jmesh_save_read(JMESH_SAVE_ROLE,sizeof(jmesh_device_role_t),(unsigned char *)&jmesh_device_roles)){
        jmesh_device_roles.water=0;
        jmesh_device_roles.gate=0;
    }
    if(jmesh_device_roles.water){
        water_init();
    }
    if(jmesh_device_roles.gate){
        water_gate_init();
    }
    water_server_init();
    water_client_init();

}
void jmesh_set_device_roles(jmesh_device_role_t device_roles)
{
    jmesh_device_roles=device_roles;
    jmesh_save_write(JMESH_SAVE_ROLE,sizeof(jmesh_device_role_t),(unsigned char *)&jmesh_device_roles);
    if(jmesh_device_roles.water){
        water_init();
        //water_server_init();
        //water_client_init();
    }
    if(jmesh_device_roles.gate){
        water_gate_init();
    }
}
jmesh_device_role_t jmesh_get_device_roles(void)
{
    return jmesh_device_roles;
}
void jmesh_app_expand2_init(void)
{

    #ifdef JMESH_APP_EXPAND2_REALCOMM_CLIENT
	jmesh_app_expand2_realcomm_client_init();
	#endif

    #ifdef JMESH_APP_EXPAND2_REALCOMM_SERVER
	jmesh_app_expand2_realcomm_server_init();
	#endif

	  #ifdef JMESH_APP_EXPAND2_WATER_SERVER
    //water_init();
    //water_server_init();
    #endif

    #ifdef JMESH_APP_EXPAND2_WATER_CLIENT
    //water_client_init();
    #endif

    #ifdef JMESH_APP_EXPAND2_WATER_GATE
    //water_gate_init();
    #endif // JMESH_APP_EXPAND2_WATER_GATE
    jmesh_device_roles_init();

}
