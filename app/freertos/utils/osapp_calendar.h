/**
 ****************************************************************************************
 *
 * @file   osapp_calendar.h
 *
 * @brief  .
 *
 * @author  ZHAOYUNLIU
 * @date    2019-01-11 15:28
 * @version <0.0.0.1>
 *
 * @license 
 *              Copyright (C) Apollo 2019
 *                         ALL Right Reserved.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup MESH_osapp_calendar_API Mesh osapp_time API
 * @ingroup MESH_API
 * @brief Mesh osapp_time  API
 *
 * @{
 ****************************************************************************************
 */

#ifndef APP_FREERTOS_UTILS_OSAPP_CALENDAR_H_
#define APP_FREERTOS_UTILS_OSAPP_CALENDAR_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
//#include "mesh_errors.h"
//#include "sdk_mesh_config.h"
//#include "sdk_mesh_definitions.h"

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
typedef struct
{
    int   ms;
    int   sec;
    int   min;
    int   hour;
    int   mday;
    int   mon;
    int   year;
}tm_date;

int set_system_time(tm_date stm);
int get_system_time(tm_date *stm);
uint64_t time_set_date_to_tai_seconds(tm_date *stm);


#endif /* APP_FREERTOS_UTILS_OSAPP_CALENDAR_H_ */ 
/// @} MESH_osapp_calendar_API

