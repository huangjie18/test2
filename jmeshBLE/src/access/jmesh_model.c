#include"jmesh_model.h"
#include"jmesh_element.h"
#include"../jmesh/jmesh_addr.h"
#include"../jmesh/jmesh_save.h"
#include"../jmesh/jmesh_types.h"
#include"../jmesh/jmesh_features.h"
#include"../jmesh/jmesh_print.h"
#include"../foundation/state/jmesh_subscription_list.h"
#include"../foundation/state/jmesh_bind.h"
#include"stdio.h"


jmesh_model_t* jmesh_model_get_by_vendor_id(jmesh_element_t* element,unsigned short company_id,unsigned short model_id)
{
    jmesh_model_t* list;
    list=element->model_list;
    while(list!=NULL){
        if(list->info->model_id.company_id==company_id&&list->info->model_id.model_id==model_id)  return list;
        list=list->next;
    }
    return list;
}

jmesh_model_t* jmesh_model_get_by_sig_id(jmesh_element_t* element,unsigned short sig_id)
{
    return jmesh_model_get_by_vendor_id(element,sig_id,JMESH_MODEL_INVALID_ID);
}

int jmesh_model_addr_check(jmesh_model_t* model,unsigned short element_addr,unsigned short addr){

    if(addr<=JMESH_ADDR_UNICAST_MAX){
        if(element_addr==addr){
            return 1;
        }
        return 0;
    }
    if(addr==JMESH_ADDR_ALL){
        return 1;
    }
    if(addr==JMESH_ADDR_ALL_PROXIES&&JMESH_FEATURE_ENABLE==jmesh_get_proxy_feature()){
        return 1;
    }
    if(addr==JMESH_ADDR_ALL_FRIENDS&&JMESH_FEATURE_ENABLE==jmesh_get_friend_feature()){
        return 1;
    }
    if(addr==JMESH_ADDR_ALL_RELAYS&&JMESH_FEATURE_ENABLE==jmesh_get_relay_feature()){
        return 1;
    }
    return jmesh_subscription_list_is_in(&model->para.subscription_list,addr);
}
void jmesh_model_subscription_list_init(jmesh_model_t* model){
    jmesh_subscription_list_init(&model->para.subscription_list);
}
int jmesh_model_subscription_list_add(jmesh_model_t* model,unsigned short addr){
    int ret = JMESH_STATUS_STORAGE_FAILURE;
    if(JMESH_STATUS_SUCCESS==(ret=jmesh_subscription_list_add(&model->para.subscription_list,addr))){
        jmesh_save_buffer_write(JMESH_SAVE_MODEL,model->id,sizeof(jmesh_model_para_t),(unsigned char*)&model->para);
    }
    return ret;
}
int jmesh_model_subscription_list_remove(jmesh_model_t* model,unsigned short addr){
    int ret;
    if(JMESH_STATUS_SUCCESS==(ret=jmesh_subscription_list_remove(&model->para.subscription_list,addr))){
        jmesh_save_buffer_write(JMESH_SAVE_MODEL,model->id,sizeof(jmesh_model_para_t),(unsigned char*)&model->para);
    }
    return ret;
}
int jmesh_model_subscription_list_overwrite(jmesh_model_t* model,unsigned short addr){
    int ret;
    if(JMESH_STATUS_SUCCESS==(ret=jmesh_subscription_list_overwrite(&model->para.subscription_list,addr))){
        jmesh_save_buffer_write(JMESH_SAVE_MODEL,model->id,sizeof(jmesh_model_para_t),(unsigned char*)&model->para);
    }
    return ret;
}
int jmesh_model_subscription_list_delete_all(jmesh_model_t* model){
    int ret;
    if(JMESH_STATUS_SUCCESS==(ret=jmesh_subscription_list_delete_all(&model->para.subscription_list))){
        jmesh_save_buffer_write(JMESH_SAVE_MODEL,model->id,sizeof(jmesh_model_para_t),(unsigned char*)&model->para);
    }
    return ret;
}
int jmesh_model_subscription_list_is_in(jmesh_model_t* model,unsigned short addr){
    return jmesh_subscription_list_is_in(&model->para.subscription_list,addr);
}
int jmesh_model_subscription_list(jmesh_model_t* model,unsigned char* ret_buffer)
{
		return jmesh_subscription_list(&model->para.subscription_list,ret_buffer);
}
void jmesh_model_bind_init(jmesh_model_t* model){
    jmesh_bind_init(&model->para.bind_app);
}
int jmesh_model_bind_app(jmesh_model_t* model,jmesh_appkey_t* appkey){
    int ret = JMESH_STATUS_CANNOT_BIND;
    if(JMESH_STATUS_SUCCESS==(ret=jmesh_bind_app(&model->para.bind_app,appkey))){
        jmesh_save_buffer_write(JMESH_SAVE_MODEL,model->id,sizeof(jmesh_model_para_t),(unsigned char*)&model->para);
    }
    return ret;
}
int jmesh_model_unbind_app(jmesh_model_t* model,jmesh_appkey_t* appkey){
    int ret;
    if(JMESH_STATUS_SUCCESS==(ret=jmesh_unbind_app(&model->para.bind_app,appkey))){
        jmesh_save_buffer_write(JMESH_SAVE_MODEL,model->id,sizeof(jmesh_model_para_t),(unsigned char*)&model->para);
    }
    return ret;
}
int jmesh_model_is_bind(jmesh_model_t* model,jmesh_appkey_t* appkey){
    return jmesh_is_bind(&model->para.bind_app,appkey);
}
int jmesh_model_bind_list(jmesh_model_t* model,unsigned char buffer_size,unsigned char* buffer){
    return jmesh_bind_list(&model->para.bind_app,buffer_size,buffer);
}

