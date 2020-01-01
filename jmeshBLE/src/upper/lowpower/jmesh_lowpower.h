#ifndef JMESH_LOWPOWER_H
#define JMESH_LOWPOWER_H
#include"../../jmesh/jmesh_config.h"
#include"../../jmesh/jmesh_pdu.h"

#define JMESH_LOWPOWER_POLL_RETRY_TIMES 10
#define JMESH_LOWPOWER_ADV_REPEATS 10
#define JMESH_LOWPOWER_POLL_INTERVAL_S 15
#define JMESH_LOWPOWER_REQST_INTERVAL_S 60

#define JMESH_LOWPOWER_POLL_INTVAL_OFFSET_S 10
#define JMESH_LOWPOWER_POLL_TIMEOUT_MS 2000

#define JMESH_LOWPOWER_SCAN_ENABLE_MS 1000
#define JMESH_LOWPOWER_SUBS_LISTUP_TIMEOUT_MS 4000
#define JMESH_LOWPOWER_UPDATE_TIMEOUT_MS 5000

#define JMESH_LOWPOWER_RECV_OFFER_DELAY_MS 100

#define JMESH_LOWPOWER_REQST_INCREAMENT_S 10
typedef struct st_lowpower_param{
    unsigned char adv_repeats;
    unsigned short poll_intval_s;
    unsigned short reqst_intval_s;
    unsigned char poll_retry_times;
}lowpower_param_t;
extern lowpower_param_t lowpower_param;

void jmesh_lowpower_init(void);
void jmesh_lowpower_uninit(void);
void jmesh_lowpower_sleep(void);
void jmesh_lowpower_wakeup_callback(void);
void jmesh_lowpower_recv_pdu_callback(void);
void jmesh_lowpower_recv(unsigned short netkey_indexz,unsigned short length,jmesh_pdu_t* pdu);

void jmesh_lowpower_subscription_list_add(unsigned short addr);
void jmesh_lowpower_subscription_list_remove(unsigned short addr);

void jmesh_lowpower_connected(unsigned short src);

int jmesh_lowpower_cache_push(jmesh_pdu_t* pdu);

#endif // JMESH_LOWPOWER_H
