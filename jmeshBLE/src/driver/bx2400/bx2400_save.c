#include"../jmesh_driver_config.h"
#if (JMESH_DRIVER_BX2400)
#include"../jmesh_save.h"
#include"jmesh_state.h"
#include"stdio.h"
//#include"nvds.h"
#include"nvds_in_ram.h"
#include"co_math.h"
#include"periph_error.h"
#include"flash_wrapper.h"
int jmesh_save_write(signed short name, unsigned short length, unsigned char *data){
    if(NVDS_OK==nvds_put(name+TAG_APP_SPECIFIC_FIRST,length,data))
    {
        //nvds_write_through();
        return 0;
    }
    return -1;
}
int jmesh_save_read(signed short name, unsigned short length, unsigned char *data){
    nvds_tag_len_t len=length;
    if(NVDS_OK==nvds_get(name+TAG_APP_SPECIFIC_FIRST,&len,data))
    {
        return len;
    }
    return 0;
}
void jmesh_save_delete(signed short name){
    nvds_del(name+TAG_APP_SPECIFIC_FIRST);
}
#include"jmesh_print.h"
void jmesh_save_init(void)
{
    uint8_t sector_num = CEILING(0x2000,0x1000);
    uint8_t i;
    uint32_t offset=0x1000;
    JMESH_LOGW(1,"sector num","%d\n",sector_num);
    for(i=0;i<sector_num;++i)
    {
        if(PERIPH_NO_ERROR!=flash_erase(offset,Sector_Erase))
        {
            JMESH_LOGW(1,"erase error","sector num%d\n",i);
        }
        offset += 0x1000;
    }
}

#endif
