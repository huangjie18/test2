#include"jmesh_seq.h"
#include"../jmesh/jmesh_config.h"
#include"../jmesh/jmesh_save_define.h"
#include"../jmesh/jmesh_save.h"
static jmesh_seq_list_t seq_list[JMESH_SEQ_LIST_SIZE];
static unsigned char seq_low_list[JMESH_SEQ_LIST_SIZE];
static unsigned long local_seq;

unsigned long jmesh_seq_get(void){
    return local_seq;
}
void jmesh_seq_add(void){
	static int i=0;
    local_seq++;
    if(++i >1000) {
      i=0;
      jmesh_save_write(JMESH_SAVE_SEQ,4,(unsigned char*)&local_seq);
	}
}
void jmesh_seq_set_local(unsigned long seq)
{
	local_seq = seq;
	jmesh_save_write(JMESH_SAVE_SEQ,4,(unsigned char*)&local_seq);
}
void jmesh_seq_list_init(void)
{
    memset(seq_list,0,sizeof(seq_list));
    memset(seq_low_list,0,sizeof(seq_low_list));
	  if(4!=jmesh_save_read(JMESH_SAVE_SEQ,4,(unsigned char*)&local_seq)){
  	    local_seq=0;
	  }
	  local_seq+=1000;
	  jmesh_save_write(JMESH_SAVE_SEQ,4,(unsigned char*)&local_seq);
}
void jmesh_seq_list_clear(void)
{
    memset(seq_list,0,sizeof(seq_list));
    memset(seq_low_list,0,sizeof(seq_low_list));
}
void jmesh_seq_set(unsigned short src,unsigned char ivi,unsigned long seq)
{
    int i;
    for(i=0;i<JMESH_SEQ_LIST_SIZE;i++){
        if(seq_list[i].addr==src){
            seq_list[i].seq_high=seq>>8;
            seq_list[i].ivi=ivi;
            seq_low_list[i]=(unsigned char)seq;
            return;
        }
    }

    for(i=0;i<JMESH_SEQ_LIST_SIZE;i++){
        if(seq_list[i].addr==0){
            seq_list[i].addr=src;
            seq_list[i].ivi=ivi;
            seq_list[i].seq_high=seq>>8;
            seq_low_list[i]=(unsigned char)seq;
            //TODO: flash write
            return ;
        }
    }
}
void jmesh_seq_remove(unsigned short src)
{
    int i;
    for(i=0;i<JMESH_SEQ_LIST_SIZE;i++){
        if(seq_list[i].addr==src){
            seq_list[i].addr=0;
            seq_list[i].seq_high=0;
            seq_list[i].ivi=0;
            seq_low_list[i]=0;
            return;
        }
    }
}
int jmesh_seq_is_valid(unsigned short src,unsigned char ivi,unsigned char active_ivi,unsigned long seq)
{
    int i;
    unsigned long last_seq;
    for(i=0;i<JMESH_SEQ_LIST_SIZE;i++){
        if(seq_list[i].addr==src){
            last_seq=seq_list[i].seq_high;
            last_seq<<=8;
            last_seq+=seq_low_list[i];

            if((seq_list[i].ivi == ivi && last_seq  < seq)
               ||(seq_list[i].ivi != ivi && active_ivi == ivi)){//iv index updated
                return 1;
            }
            return 0;
        }
    }
    return 1;
}
