#ifndef LIGHT_SERVER_H
#define LIGHT_SERVER_H


typedef struct st_light_mode_param{
    unsigned short speed_ticks;
    unsigned short on_ticks;
    unsigned short off_ticks;
    unsigned short live_ticks;
    unsigned char direction;//0-reverse,1-
}light_mode_param_t;

enum{
    LIGHT_MODE_SET_ERROR_OK=0,
    LIGHT_MODE_SET_ERROR_ID=1,
    LIGHT_MODE_SET_ERROR_LEN=2,
};

void light_server_init(void);

#define LIGHT_MODE_SIZE 5

#endif // LIGHT_SERVER_H
