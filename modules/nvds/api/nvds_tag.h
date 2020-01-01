#ifndef NVDS_TAG_H_
#define NVDS_TAG_H_
#include "nvds_typedef.h"

#define DECL_TAG(tag,array_type,total_length)   \
        struct{                                 \
            TAG_HEAD;                           \
            array_type data[(total_length)/sizeof(array_type)]  __attribute__((packed));      \
        } tag##_env __attribute__((packed))
#define DECL_END() uint8_t tail

typedef struct {
    __attribute__((packed)) nvds_data_head_t head;
    DECL_TAG(NVDS_TAG_DEVICE_NAME,uint8_t,NVDS_LEN_DEVICE_NAME);
    DECL_TAG(NVDS_TAG_BD_ADDRESS,uint8_t,NVDS_LEN_BD_ADDRESS);
    DECL_TAG(NVDS_TAG_EXT_WAKEUP_TIME,uint16_t,NVDS_LEN_EXT_WAKEUP_TIME);
    DECL_TAG(NVDS_TAG_OSC_WAKEUP_TIME,uint16_t,NVDS_LEN_OSC_WAKEUP_TIME);
    DECL_TAG(NVDS_TAG_RM_WAKEUP_TIME,uint16_t,NVDS_LEN_RM_WAKEUP_TIME);
    DECL_TAG(NVDS_TAG_SLEEP_ENABLE,uint8_t,NVDS_LEN_SLEEP_ENABLE);
    DECL_TAG(NVDS_TAG_EXT_WAKEUP_ENABLE,uint8_t,NVDS_LEN_EXT_WAKEUP_ENABLE);
    DECL_TAG(NVDS_TAG_LPCLK_DRIFT,uint16_t,NVDS_LEN_LPCLK_DRIFT);
    //DECL_TAG(NVDS_TAG_DIAG_BLE_HW,uint8_t,NVDS_LEN_DIAG_BLE_HW);
    
    DECL_END();
}nvds_data_t;

#endif
