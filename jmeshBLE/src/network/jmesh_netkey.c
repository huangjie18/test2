#include"jmesh_netkey.h"
#include"../jmesh/jmesh_save.h"
#include"../secure/jmesh_security.h"
#include"jmesh_seq.h"



static jmesh_netkey_t jmesh_netkey[JMESH_NETKEY_MAX_NUM];

static void jmesh_netkey_load(jmesh_netkey_t* netkey){

    netkey->cache.nid[0]=jmesh_security_cal_master_encryption_and_privacy_keys(netkey->key[0],netkey->cache.encryption_key[0],netkey->cache.privacy_key[0]);
    jmesh_security_cal_netid(netkey->key[0],netkey->cache.netid[0]);
    jmesh_security_cal_identity_key(netkey->key[0],netkey->cache.identity_key[0]);
    jmesh_security_cal_beacon_key(netkey->key[0],netkey->cache.beacon_key[0]);

    netkey->cache.nid[1]=jmesh_security_cal_master_encryption_and_privacy_keys(netkey->key[1],netkey->cache.encryption_key[1],netkey->cache.privacy_key[1]);
    jmesh_security_cal_netid(netkey->key[1],netkey->cache.netid[1]);
    jmesh_security_cal_identity_key(netkey->key[1],netkey->cache.identity_key[1]);
    jmesh_security_cal_beacon_key(netkey->key[1],netkey->cache.beacon_key[1]);
}

void jmesh_netkey_1min_schedule(void){
    int i;
    for(i=0;i<JMESH_NETKEY_MAX_NUM;i++){
        if(jmesh_netkey[i].min_count_timer){
            jmesh_netkey[i].min_count_timer--;

            if(jmesh_netkey[i].min_count_timer==0){
                jmesh_netkey_stop_update_iv_index(0);
            }
            //TODO:save may
        }
    }
}
void jmesh_netkey_init(void){
    int i;
    jmesh_netkey_t key;
    for(i=0;i<JMESH_NETKEY_MAX_NUM;i++){
        memset(&jmesh_netkey[i],0,sizeof(jmesh_netkey_t));
        if(JMESH_NETKEY_SAVE_LENGTH==jmesh_save_buffer_read(JMESH_SAVE_NETKEY,i,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)&key)){
            jmesh_netkey[i]=key;
        }
        if(jmesh_netkey[i].iv_index){
            jmesh_netkey_load(&jmesh_netkey[i]);
        }
    }
}

int jmesh_netkey_state_set(unsigned short index,unsigned char* key)
{
    int i,j;
    jmesh_netkey_t netkey;

    netkey.index=index;
    memcpy(netkey.key,key,JMESH_KEY_SIZE);

    for(i=0,j=-1;i<JMESH_NETKEY_MAX_NUM;i++){
        if(JMESH_NETKEY_IS_VALID(&jmesh_netkey[i]) && index==JMESH_NETKEY_GET_INDEX(&jmesh_netkey[i])){
            if(0!=memcmp(key,JMESH_NETKEY_GET_KEY(&jmesh_netkey[i]),JMESH_KEY_SIZE)){

                jmesh_netkey[i].new_key_index=0;
                jmesh_netkey[i].min_count_timer=0;
                jmesh_netkey[i].phase=0;
                memcpy(jmesh_netkey[i].key[0],key,JMESH_KEY_SIZE);
                jmesh_netkey_load(&jmesh_netkey[i]);
                jmesh_save_buffer_write(JMESH_SAVE_NETKEY,i,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)&jmesh_netkey[i]);
            }
            return JMESH_STATUS_SUCCESS;
        }
        if(j<0){
            if(!JMESH_NETKEY_IS_VALID(&jmesh_netkey[i])){
                j=i;
            }
        }
    }
    if(j>=0){
        jmesh_netkey[j].index=index;
        jmesh_netkey[j].iv_index=JMESH_INIT_IV_INDEX;
        jmesh_netkey[j].node_identity=JMESH_DEFAULT_NODE_IDENTITY;
        jmesh_netkey[j].new_key_index=0;
        jmesh_netkey[j].min_count_timer=0;
        jmesh_netkey[j].phase=0;
        memcpy(jmesh_netkey[j].key[0],key,JMESH_KEY_SIZE);
        jmesh_netkey_load(&jmesh_netkey[j]);
        jmesh_save_buffer_write(JMESH_SAVE_NETKEY,j,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)&jmesh_netkey[j]);

        return JMESH_STATUS_SUCCESS;
    }
    return JMESH_STATUS_STORAGE_FAILURE;
}
int jmesh_netkey_state_get(unsigned short index,unsigned char* key)
{
    int i;
    for(i=0;i<JMESH_NETKEY_MAX_NUM;i++){
        if(JMESH_NETKEY_IS_VALID(&jmesh_netkey[i]) && index==JMESH_NETKEY_GET_INDEX(&jmesh_netkey[i])){
            memcpy(key,JMESH_NETKEY_GET_KEY(&jmesh_netkey[i]),JMESH_KEY_SIZE);
            return JMESH_STATUS_SUCCESS;
        }
    }
    return JMESH_STATUS_INVALID_NETKEY_INDEX;
}
int jmesh_netkey_state_update(unsigned short index,unsigned char* key){
    int i;
    for(i=0;i<JMESH_NETKEY_MAX_NUM;i++){
        if(JMESH_NETKEY_IS_VALID(&jmesh_netkey[i]) && index==JMESH_NETKEY_GET_INDEX(&jmesh_netkey[i])){
            if(0==memcmp(key,JMESH_NETKEY_GET_KEY(&jmesh_netkey[i]),JMESH_KEY_SIZE)){
                return JMESH_STATUS_SUCCESS;
            }
            if(jmesh_netkey[i].phase==0){
                jmesh_netkey[i].phase=1;
                memcpy(JMESH_NETKEY_GET_OLD_KEY(&jmesh_netkey[i]),key,JMESH_KEY_SIZE);
                jmesh_netkey_load(&jmesh_netkey[i]);
                jmesh_save_buffer_write(JMESH_SAVE_NETKEY,i,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)&jmesh_netkey[i]);
                return JMESH_STATUS_SUCCESS;
            }
        }
    }
    return JMESH_STATUS_SUCCESS;
}

int jmesh_netkey_state_del(unsigned short index){
    int i;
    for(i=0;i<JMESH_NETKEY_MAX_NUM;i++){
        if(JMESH_NETKEY_IS_VALID(&jmesh_netkey[i]) && index==JMESH_NETKEY_GET_INDEX(&jmesh_netkey[i])){
            JMESH_NETKEY_SET_INVALID(&jmesh_netkey[i]);
            jmesh_save_buffer_delete(JMESH_SAVE_NETKEY,i);
            return JMESH_STATUS_SUCCESS;
        }
    }
    return JMESH_STATUS_INVALID_NETKEY_INDEX;
}

jmesh_netkey_t* jmesh_netkey_get_by_nid(unsigned char nid,jmesh_netkey_t* prev_key){
    int i;
    if(prev_key==NULL){
        i=-1;
    }
    else{
        i=prev_key-jmesh_netkey;
    }
    for(i++;i<JMESH_NETKEY_MAX_NUM;i++){
        if( JMESH_NETKEY_IS_VALID(&jmesh_netkey[i])&& JMESH_NETKEY_GET_NID(&jmesh_netkey[i])==nid){
            return &jmesh_netkey[i];
        }
    }
    return NULL;
}
jmesh_netkey_t* jmesh_netkey_get_by_index(unsigned short index){
    int i;
    for(i=0;i<JMESH_NETKEY_MAX_NUM;i++){
        if(JMESH_NETKEY_IS_VALID(&jmesh_netkey[i]) && index==JMESH_NETKEY_GET_INDEX(&jmesh_netkey[i])){
            return &jmesh_netkey[i];
        }
    }
    return NULL;
}

int jmesh_netkey_state_list(unsigned char buffer_size,unsigned char * pdu){

    int list_index,pdu_index;
    for(list_index=0,pdu_index=0;list_index<JMESH_NETKEY_MAX_NUM && (pdu_index+3<=buffer_size);list_index++){
        if(JMESH_NETKEY_IS_VALID(&jmesh_netkey[list_index])){
            if((pdu_index%3)==0){
                pdu[pdu_index]=jmesh_netkey[list_index].index;
                pdu_index+=1;
                pdu[pdu_index]=jmesh_netkey[list_index].index>>8;
                pdu_index+=1;
            }
            else{
                pdu[pdu_index-1]|=jmesh_netkey[list_index].index<<4;
                pdu[pdu_index]=jmesh_netkey[list_index].index>>4;
                pdu_index+=1;
            }
        }
    }
    return pdu_index;
}
int jmesh_netkey_state_transtion(unsigned short index,unsigned char transition){
    jmesh_netkey_t* netkey;
    if(NULL==(netkey=jmesh_netkey_get_by_index(index))){
        return JMESH_STATUS_INVALID_NETKEY_INDEX;
    }
    if(transition==2){
        if(netkey->phase==1){
            netkey->phase=2;
            netkey->new_key_index=~netkey->new_key_index;
            jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
            return JMESH_STATUS_SUCCESS;
        }
    }
    else if(transition==3){
        if(netkey->phase==1||netkey->phase==2){
            netkey->phase=0;
            jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
            return JMESH_STATUS_SUCCESS;
        }
    }
    return JMESH_STATUS_CANNOT_SET;
}
int jmesh_netkey_state_set_node_identity(unsigned short index,unsigned char identity){
    jmesh_netkey_t* netkey;
    if(NULL==(netkey=jmesh_netkey_get_by_index(index))){
        return JMESH_STATUS_INVALID_NETKEY_INDEX;
    }
    netkey->node_identity=identity;
    return JMESH_STATUS_SUCCESS;
}
int jmesh_netkey_state_get_node_identity(unsigned short index,unsigned char *identity){
    jmesh_netkey_t* netkey;
    if(NULL==(netkey=jmesh_netkey_get_by_index(index))){
        return JMESH_STATUS_INVALID_NETKEY_INDEX;
    }
    *identity=netkey->node_identity;
    return JMESH_STATUS_SUCCESS;
}

unsigned char jmesh_netkey_get_iv_update_flag(unsigned short netkey_index)
{
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        return netkey->iv_update_flag;
    }
    return 0;
}
unsigned long jmesh_netkey_get_iv_index(unsigned short netkey_index)
{
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        return netkey->iv_index;
    }
    return JMESH_INVALID_IV_INDEX;
}

void jmesh_netkey_start_update_iv_index(unsigned short netkey_index)
{
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        if(netkey->min_count_timer==0){
            netkey->min_count_timer=JMESH_IV_INDEX_UPDATE_LASTING_HOUR*60;
            netkey->iv_update_flag=1;
            netkey->iv_index++;
            jmesh_seq_set_local(1);
            jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
        }
    }
}
void jmesh_netkey_stop_update_iv_index(unsigned short netkey_index)
{
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        if(netkey->min_count_timer==0&&netkey->iv_update_flag){
            netkey->min_count_timer=JMESH_IV_INDEX_UPDATE_INTERVAL_HOUR;
            netkey->iv_update_flag=0;
            jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
        }
    }
}
void jmesh_netkey_recover_iv_index(unsigned short netkey_index,unsigned long iv_index)
{
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        if(netkey->min_count_timer==0&&iv_index-netkey->iv_index<JMESH_IV_INDEX_RECOVER_GREATER_MAX){
            netkey->min_count_timer=JMESH_IV_INDEX_RECOVER_INTERVAL_HOUR;
            netkey->iv_index=iv_index;
            if(netkey->iv_update_flag){
                netkey->iv_update_flag=0;
            }
            jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
        }
    }
}
int jmesh_netkey_test_set_iv_index(unsigned short netkey_index,unsigned long iv_index){
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        netkey->iv_index=iv_index;
        jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
        return 0;
    }
    return -1;
}
int jmesh_netkey_test_start_update_iv_index(unsigned short netkey_index){
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        netkey->iv_update_flag=1;
        netkey->iv_index++;
        jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
        return 0;
    }
    return -1;
}
int jmesh_netkey_test_stop_update_iv_index(unsigned short netkey_index){
    jmesh_netkey_t* netkey;
    if(NULL!=(netkey=jmesh_netkey_get_by_index(netkey_index))){
        netkey->iv_update_flag=0;
        jmesh_save_buffer_write(JMESH_SAVE_NETKEY,netkey-jmesh_netkey,JMESH_NETKEY_SAVE_LENGTH,(unsigned char*)netkey);
        return 0;
    }
    return -1;
}
