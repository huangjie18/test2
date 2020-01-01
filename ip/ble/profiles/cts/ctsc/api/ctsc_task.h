#ifndef CTSC_TASK_H_
#define CTSC_TASK_H_

#include "rwip_task.h"
#include "prf_types.h"
#include "ke.h"
#include "ke_mem.h"

enum ctsc_cts_chars
{
    CTS_CHAR_CURRENT_TIME,
    CTS_CHAR_LOCAL_TIME_INFO,
    CTS_CHAR_REF_TIME_INFO,
    CTS_CHAR_MAX,
};

enum ctsc_cts_descs
{
    CTS_DESC_CURRENT_TIME_CL_CFG,
    CTS_DESC_MAX,    
};

enum
{
    CTSC_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_CTSC),
    
    CTSC_ENABLE_RSP,
    
    CTSC_GET_CURRENT_TIME_REQ,
    
    CTSC_GET_CURRENT_TIME_RSP,
};

struct ctsc_cts_content
{
    struct prf_svc svc;
    struct prf_char_inf chars[CTS_CHAR_MAX];
    struct prf_char_desc_inf descs[CTS_DESC_MAX];
};

struct ctsc_enable_req
{
    uint8_t con_type;
    struct ctsc_cts_content cts;
};

struct ctsc_enable_rsp
{
    uint8_t status;
    struct ctsc_cts_content cts;
};

/**@brief "Exact Time 256" field of the Current Time Characteristic. */
typedef struct __attribute__((packed)) 
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
    uint8_t  day_of_week;
    uint8_t  fractions256;
} exact_time_256_t;

/**@brief "Adjust Reason" field of the Current Time Characteristic. */
typedef struct __attribute__((packed)) 
{
    uint8_t manual_time_update              : 1;
    uint8_t external_reference_time_update  : 1;
    uint8_t change_of_time_zone             : 1;
    uint8_t change_of_daylight_savings_time : 1;
} adjust_reason_t;

/**@brief Data structure for the Current Time Characteristic. */
typedef struct __attribute__((packed)) 
{
    exact_time_256_t exact_time_256;
    adjust_reason_t  adjust_reason;
} current_time_char_t;

struct ctsc_get_current_time_rsp
{
    current_time_char_t ct;
    uint8_t status;
};

struct ctsc_get_current_time_req
{
    //empty
};

extern const struct ke_state_handler ctsc_default_handler;

#endif
