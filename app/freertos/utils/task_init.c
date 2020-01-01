 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "bx_config.h"
#include "osapp_config.h"
#include "rwip_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "task_init.h"
#include "rwip.h"
#include "bx_dbg.h"
#include "log.h"
#include "arch.h"
#include "ble_task.h"
#include "osapp_task.h"
#include "gap.h"
#include "gapm_task.h"
#include "apollo_00.h"
#include "awo.h"
#include "rtc.h"
#include "log.h"
#include "rst_gen.h"
#include "plf.h"
#include "reg_sysc_awo.h"
#include "sys_sleep.h"
#include "rc_calib.h"
#include "compiler_flag.h"
#if defined( INCLUDE_uxTaskGetStackHighWaterMark )
extern void stk_chk_dbg( void );
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#if (RC32K_USED == 1)
#define portRTC_FREQUENCY (1000000*64*32)/get_rc32k_calib_val()
#else
#if (HZ32000)
#define portRTC_FREQUENCY 32000
#else
#define portRTC_FREQUENCY 32768
#endif
#endif

static uint32_t ulTimerCountsForOneTick;
static uint32_t ulTimerCountsRemainder;
static uint32_t RTCWakeupDelay;
static uint32_t rtcMatchCount;
#define RTC_INITIAL_VAL 0xfff00000


/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
/*
#if defined( INCLUDE_uxTaskGetStackHighWaterMark )
void stk_chk_dbg( void )
{
	unsigned portBASE_TYPE uxHighWaterMark;
	uint8_t task_num,i;

	task_num = core_task_table_info.table_size;
    const task_table_t *task_table = core_task_table_info.task_table;
	for(i=0; i < task_num; i++)
	{
		if(task_table[i].is_used == true)
		{
			BX_ASSERT(task_table[i].task_handle != NULL);									// task should already been initialized

			uxHighWaterMark = uxTaskGetStackHighWaterMark(*task_table[i].task_handle);		// null means check current task stack
			LOG(LOG_LVL_INFO,"%s stk remain 0x%x*4Byte\n",task_table[i].task_name,uxHighWaterMark);
		}
	}

    
    task_num = extra_task_table_info.table_size;
    task_table = extra_task_table_info.task_table;
    for(i=0; i < task_num; i++)
    {
        if(task_table[i].is_used == true)
        {
            BX_ASSERT(task_table[i].task_handle != NULL);                                   // task should already been initialized

            uxHighWaterMark = uxTaskGetStackHighWaterMark(*task_table[i].task_handle);      // null means check current task stack
            LOG(LOG_LVL_INFO,"%s stk remain 0x%x*4Byte\n",task_table[i].task_name,uxHighWaterMark);
        }
    }

}
#endif
*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if malloc failed */
    LOG(LOG_LVL_ERROR,"Malloc Failed\n");
    __BKPT(2);
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* vApplicationStackOverflowHook will only be called if stackoverflow */
    LOG(LOG_LVL_ERROR,"StackOverFlow\n");
    __BKPT(2);
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

bool msg_send(QueueHandle_t q,void *data,uint32_t xTicksToWait)
{
    return xQueueSend(q,data,xTicksToWait);
}

N_XIP_SECTION bool msg_send_isr(QueueHandle_t q,void *data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    bool retval = xQueueSendFromISR(q,data,&xHigherPriorityTaskWoken);
    if(retval)
    {
       portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return retval;
}

//extern void mesh_task_create(void* param);
void rtos_task_init()
{
    ble_queue_create();
    app_queue_create();
    ble_stack_task_create(NULL);
    osapp_task_create(NULL);
    #ifdef  OSAPP_MESH
    //mesh_task_create(NULL);
    #endif
    vTaskStartScheduler();

}

N_XIP_SECTION static bool rtc_match_cnt_increase(uint32_t step)
{
    uint32_t old_rtc_match_cnt = rtcMatchCount;
    rtcMatchCount += ulTimerCountsForOneTick * step;
    #if (RC32K_USED == 0)
    static uint32_t compensate_cnt = 0;
    compensate_cnt += step;
    rtcMatchCount += ulTimerCountsRemainder * (compensate_cnt / configTICK_RATE_HZ );
    compensate_cnt %= configTICK_RATE_HZ;
    #endif
    if(rtcMatchCount< old_rtc_match_cnt)
    {
        return true;
    }else
    {
        return false;
    }
}

void xPortSysTickHandler( void );
N_XIP_SECTION void vPortOSTick_IRQ()
{
    RTC_INT_CLR();
    rtc_match_cnt_increase(1);
    /* --start-- for breakpoint debug use */
    uint32_t current_rtc_val = RTC_CURRENTCNT_GET();
    int32_t rtc_diff = current_rtc_val - rtcMatchCount;
    if(rtc_diff >= 0)
    {
        LOG(LOG_LVL_WARN,"missing ticks\n");
        uint32_t step_ticks = (current_rtc_val - rtcMatchCount) / ulTimerCountsForOneTick + 1;
        rtc_match_cnt_increase(step_ticks);
    }
    /* --end-- */
    RTC_MATCHCNT_SET(rtcMatchCount);
    xPortSysTickHandler();
}

N_XIP_SECTION void RTC_IRQHandler(void)
{
    vPortOSTick_IRQ();
}

static void wakeup_delay_and_timer_counts_config()
{
    RTCWakeupDelay = rwip_us_2_lpcycles(FREERTOS_WAKEUP_DELAY);
    ulTimerCountsForOneTick = portRTC_FREQUENCY / configTICK_RATE_HZ;
    ulTimerCountsRemainder = portRTC_FREQUENCY % configTICK_RATE_HZ;
    #if(RC32K_USED == 1)
    if(ulTimerCountsRemainder * 2 >= configTICK_RATE_HZ)
    {
        ulTimerCountsForOneTick += 1;
    }
    #endif
}

void system_tick_init()
{
    wakeup_delay_and_timer_counts_config();
    rtcMatchCount = RTC_INITIAL_VAL;
    rtc_match_cnt_increase(1);
    RTC_MATCHCNT_SET(rtcMatchCount);
    RTC_LOADCNT_SET(RTC_INITIAL_VAL);
    RTC_EN_WITH_INT();

}

static void clr_rtc_intr()
{   
    RTC_INT_CLR();
    NVIC_ClearPendingIRQ(RTC_IRQn);
}

static void os_tick_and_rtc_matchcnt_update(uint32_t step)
{
    vTaskStepTick(step);
    rtc_match_cnt_increase(step);
    RTC_MATCHCNT_SET(rtcMatchCount);
    clr_rtc_intr();
}

N_XIP_SECTION void bxSuppressTicksAndSleep(uint32_t xExpectedIdleTime )
{
    taskENTER_CRITICAL();
    uint8_t sleep_type = sleep_prepare_and_check();
    #if(RC32K_USED == 1)
    wakeup_delay_and_timer_counts_config();
    #endif
    eSleepModeStatus eSleepStatus = eTaskConfirmSleepModeStatus();
    if(eSleepStatus != eAbortSleep && sleep_type == (PROCESSOR_SLEEP|BLE_DEEP_SLEEP|SYS_DEEP_SLEEP))
    {
        uint32_t wakeupMatchValue; 
        uint32_t prevRTCMatchCount = rtcMatchCount - ulTimerCountsForOneTick;
        if(eSleepStatus == eStandardSleep)
        {
            wakeupMatchValue = prevRTCMatchCount + ulTimerCountsForOneTick*(xExpectedIdleTime+1) - RTCWakeupDelay;
        }else
        {
            wakeupMatchValue = prevRTCMatchCount - 1; // long enough before RTC IRQ raise
        }
        RTC_MATCHCNT_SET(wakeupMatchValue);
        clr_rtc_intr();
        pre_deepsleep_processing_mp();
        __WFI();
        post_deepsleep_processing_mp();
        bool rtc_int = RTC_INT_STAT_GET()? true : false;
        /* static */ uint32_t RTCValueAfterSleep;
        RTCValueAfterSleep = RTC_CURRENTCNT_GET();
        if(eSleepStatus == eStandardSleep &&  rtc_int) 
        {
            BX_ASSERT((uint64_t)wakeupMatchValue + RTCWakeupDelay >RTCValueAfterSleep );
        }
        uint32_t RTCIncrement = RTCValueAfterSleep - prevRTCMatchCount;
        uint32_t OSTickIncrement = RTCIncrement /ulTimerCountsForOneTick;
        os_tick_and_rtc_matchcnt_update(OSTickIncrement);
        /* volatile static */ uint32_t current_cnt;
        current_cnt = RTC_CURRENTCNT_GET();
        if(current_cnt == rtcMatchCount)
        {
            BX_ASSERT(rtc_int == false);
            os_tick_and_rtc_matchcnt_update(1);
        }
    }else if(sleep_type & PROCESSOR_SLEEP)
    {
        __WFI();
    }
    taskEXIT_CRITICAL();
}





