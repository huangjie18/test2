
/**
 ****************************************************************************************
 *
 * @file task_init.h
 *
 * @brief responsible for freertos task initialize 
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */

#ifndef TASK_INIT_H_
#define TASK_INIT_H_


/**
 ****************************************************************************************
 * @addtogroup ARCH
 * @ingroup APOLLO_00
 *
 * @brief task init of freertos
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>
#include <stdint.h>
#include "osmsg_queue.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


/*
 * DEFINES
 ****************************************************************************************
 */




/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void rtos_task_init(void);						// init of all freertos tasks.
void stk_chk_dbg( void );
void system_tick_init(void);
bool msg_send(QueueHandle_t q,void *data,uint32_t xTicksToWait);
bool msg_send_isr(QueueHandle_t q,void *data);


#endif // TASK_INIT_H_


