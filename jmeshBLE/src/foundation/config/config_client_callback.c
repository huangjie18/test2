#include"config_server.h"
#include"config_client.h"
#include"config_client_callback.h"
#include"../../jmesh/jmesh_print.h"

void config_appkey_list_callback(unsigned short src,unsigned short netkey_index,unsigned char len,unsigned char* appkey_index_list){
    JMESH_LOGN_BUFFER("config callback",len,appkey_index_list,"src=0x%x appkey_list:",src);
}
void config_appkey_status_callback(unsigned short src,unsigned short netkey_index,unsigned short appkey_index){
    JMESH_LOGN("config callback","appkey src=0x%x nid=0x%x aid=0x%x\n",src,netkey_index,appkey_index);
}
void config_beacon_status_callback(unsigned short src,unsigned char beacon){
    JMESH_LOGN("config callback","beacon src=0x%x,status=%d\n",src,beacon);
}
void config_composition_data_status_callback(unsigned short src,unsigned char page, unsigned short len,unsigned char* composition_data){
    JMESH_LOGN_BUFFER("config callback",len,composition_data,"composition src=0x%d page=%d data:",src,page);
}
void config_default_ttl_status_callback(unsigned short src,unsigned char ttl){

    JMESH_LOGN("config callback","default ttl src=0x%x,status=%d\n",src,ttl);
}

void config_gatt_proxy_status_callback(unsigned short src,unsigned char proxy){
    JMESH_LOGN("config callback","proxy src=0x%x,status=%d\n",src,proxy);

}
void config_friend_status_callback(unsigned short src,unsigned char friend){
    JMESH_LOGN("config callback","friend src=0x%x,status=%d\n",src,friend);
}
void config_relay_status_callback(unsigned short src,unsigned char relay){
    JMESH_LOGN("config callback","relay src=0x%x,status=%d\n",src,relay);
}

void config_model_subscription_status_callback(unsigned short src,unsigned short element,unsigned short address,jmesh_model_id_t* model_id){
    print_info("print by %s\n",__func__);
    JMESH_LOGN("config callback","subscription src=0x%x element=%d,addr=0x%x,company_Id=0x%x,model_id=0x%x\n",src,element,address,model_id->company_id,model_id->model_id);
}

void config_sig_model_subscription_list_callback(unsigned short src,unsigned short element,jmesh_model_id_t* model, unsigned short len ,unsigned char* data){
    JMESH_LOGN_BUFFER("config callback",len,data,"sig model subscription src=0x%x element=%d,sig_id=0x%x,list:",src,element,model->sig_id);
}

void config_vendor_model_subscription_list_callback(unsigned short src,unsigned short element,jmesh_model_id_t* model, unsigned short len ,unsigned char* data){
    JMESH_LOGN_BUFFER("config callback",len,data,"vendor model subscription src=0x%x element=%d,company_id=0x%x,model_id=0x%x,list:",src,element,model->company_id,model->model_id);
}
void config_netkey_status_callback(unsigned short src,unsigned short netkey_index){
    JMESH_LOGN("config client","netkey src=0x%x,index=0x%x:",src,netkey_index);
}

void config_netkey_list_callback(unsigned short src,unsigned short len ,unsigned char* data){
    JMESH_LOGN_BUFFER("config client",len,data,"netkey src=0x%x list:",src);
}

void config_model_app_bind_callback(unsigned short src,unsigned short element,unsigned short appkey_index,jmesh_model_id_t* model_id){
    JMESH_LOGN("config callback","bind src=0x%x element=%d appkey_index=0x%d company_id=0x%x,model_id=0x%x\n",src,element,appkey_index,model_id->company_id,model_id->model_id);

}
void config_sig_model_app_list_callback(unsigned short src,unsigned short element ,jmesh_model_id_t* model_id,unsigned short len ,unsigned char* data){
    JMESH_LOGN_BUFFER("config callback",len,data,"bind src=0x%x element=%d,sig_id=0x%x,key indexes:",src,element,model_id->sig_id);

}
void config_vendor_model_app_list_callback(unsigned short src,unsigned short element ,jmesh_model_id_t* model_id,unsigned short len ,unsigned char* data){
    JMESH_LOGN_BUFFER("config callback",len,data,"bind src=0x%x element=%d,company_id=0x%x,model_id=0x%x,key indexes:",src,element,model_id->company_id,model_id->model_id);

}
