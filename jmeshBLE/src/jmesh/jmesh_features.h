#ifndef JMESH_FEATURES_H
#define JMESH_FEATURES_H

#include"jmesh_config.h"

typedef struct st_jmesh_features{
    union{
        unsigned char features;
        struct{
            unsigned char proxy:2;
            unsigned char friend:2;
            unsigned char relay:2;
            unsigned char lowpower:2;
        };
    };
}jmesh_features_t;

void jmesh_features_init(void);

jmesh_features_t jmesh_get_features(void);

unsigned char jmesh_get_proxy_feature(void);
unsigned char jmesh_get_friend_feature(void);
unsigned char jmesh_get_relay_feature(void);
unsigned char jmesh_get_lowpower_feature(void);

int jmesh_set_proxy_feature(unsigned char feature);
int jmesh_set_friend_feature(unsigned char feature);
int jmesh_set_relay_feature(unsigned char feature);
int jmesh_set_lowpower_feature(unsigned char feature);


#endif // JMESH_FEATURES_H
