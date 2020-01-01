#include"jmesh_filter.h"

void jmesh_filter_init(jmesh_filter_t* filter)
{
    filter->type=JMESH_FILTER_TYPE_WHITE_LIST;
    filter->num=0;
}

void jmesh_filter_set_type(jmesh_filter_t* filter,unsigned char type)
{
    filter->type=type;
    filter->num=0;
}
void jmesh_filter_add_addr(jmesh_filter_t* filter,unsigned short addr)
{
    int i;
    for(i=0;i<filter->num;i++){
        if(filter->addr[i]==addr){
            return;
        }
    }
    if(filter->num<JMESH_FILTER_SIZE){
        filter->addr[filter->num++]=addr;
    }
}
void jmesh_filter_remove_addr(jmesh_filter_t* filter,unsigned short addr)
{
    int i;
    for(i=0;i<filter->num;i++){
        if(filter->addr[i]==addr){
            filter->num-=1;
            break;
        }
    }
    for(;i<filter->num;i++){
        filter->addr[i]=filter->addr[i+1];
    }
}
int jmesh_filter_status(jmesh_filter_t* filter,unsigned char buffer_size,unsigned char* buffer)
{
    int i,j;

    buffer[0]=filter->type;
    for(i=0,j=1;i < filter->num && (j+2) <= buffer_size;i++,j+=2){
        buffer[j]=(unsigned char)(filter->addr[i]>>8);
        buffer[j+1]=(unsigned char)filter->addr[i];
    }
    return j;
}
int jmesh_filter_get_type(jmesh_filter_t* filter)
{
    return filter->type;
}

int jmesh_filter_filtering(jmesh_filter_t* filter,unsigned short addr){
    int i;
    if(filter->type==JMESH_FILTER_TYPE_WHITE_LIST){
        for(i=0;i<filter->num;i++){
            if(filter->addr[i]==addr){
                return 1;
            }
        }
        return 0;
    }
    else{
        for(i=0;i<filter->num;i++){
            if(filter->addr[i]==addr){
                return 0;
            }
        }
        return 1;
    }
}
