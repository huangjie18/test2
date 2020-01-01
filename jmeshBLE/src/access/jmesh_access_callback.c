#include"jmesh_access_callback.h"
#include"jmesh_model.h"
#include"jmesh_element.h"
#include"jmesh_model.h"
#include"jmesh_access.h"
#include"../app/app_include.h"
#include"../foundation/config/config_client.h"
#include"../foundation/config/config_server.h"
JMESH_ELEMENT_CREATE(element_foundation);
JMESH_ELEMENT_CREATE(element_app);

void jmesh_access_init_element_callback(void)
{
    jmesh_access_add_element(&element_foundation);
    jmesh_access_add_element(&element_app);
}
void jmesh_access_init_model_callback(void){
    config_server_init();
    config_client_init();
	  app_expand_model_init();
	  light_client_init();
    light_server_init();
	  meter_client_init();
    meter_server_init();
    test_server_init();
    test_client_init();
}
