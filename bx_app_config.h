#ifndef BX_APP_CONFIG_H_
#define BX_APP_CONFIG_H_

#define OSAPP_MESH   1       //enable mesh
//#define configTOTAL_HEAP_SIZE  ((size_t) ( 15* 1024 ) )

#define ENABLE_ADV_PAYLOD_31BYTE_PATCH 1       //adv payload data max len 31byte

//#define MESH_SCHED_PATCH 1

#define CFG_PRF_DISS
#define CFG_PRF_BXOTAS
#define OTA_SWITCH    0

#define DEEP_SLEEP_ENABLE 		{0}     //disable sleep
#define EXT_WAKE_UP_ENABLE      {1}		//�͹��Ķ���Ϊ1������Ϊ0
#define RC32K_USED  1					//���͹��ĵ�·��Ӳ���޸�Ϊ0�������͹��ĵ�·��Ӳ���޸�Ϊ1
#endif
