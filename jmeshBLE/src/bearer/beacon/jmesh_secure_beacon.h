#ifndef JMESH_SECURE_BEACON_H
#define JMESH_SECURE_BEACON_H

#include"../../jmesh/jmesh_config.h"

#define JMESH_SECURE_BEACON_EXPECTED_NUMBER    (JMESH_SECURE_BEACON_OBSERVATION_PERIOD_S/JMESH_SECURE_BEACON_EXPECTED_INTERVAL_S)
#define JMESH_SECURE_BEACON_TIME_CACHE_SIZE  ((JMESH_SECURE_BEACON_MAX_INTERVAL_S*JMESH_SECURE_BEACON_EXPECTED_INTERVAL_S)/JMESH_SECURE_BEACON_OBSERVATION_PERIOD_S)


typedef struct st_jmesh_secure_beacon{
    unsigned char :6;
    unsigned char iv_update_flag    :1;//when IV updating(use IV and IV-1)
    unsigned char key_refresh_flag  :1;//when netkey phase =2
    unsigned char network_id[8];
    unsigned char iv_index[4];
    unsigned char authentication_value[8];
}jmesh_secure_beacon_t;

void jmesh_secure_beacon_start(void);
void jmesh_secure_beacon_stop(void);
void jmesh_secure_beacon_handler(jmesh_secure_beacon_t* beacon);
#endif // JMESH_SECURE_BEACON_H
