#include "apollo_00.h"
#include "app_adc.h"

#define ADC_GPADC_RO_TRIM   1
#define ADC_GPADC_SINGLE_END_MODE_EN 0
#define ADC_GPADC_DIFFERENTIAL_MODE_EN 0
#if (defined BX_TEMP_SENSOR) && (BX_TEMP_SENSOR == 1) 
#define ADC_TEMP_SENSOR_EN 1
#else
#define ADC_TEMP_SENSOR_EN 0
#endif

#if (defined ADC_GPADC_RO_TRIM) && (ADC_GPADC_RO_TRIM == 1)
#define ADC_TRIM_SIP_FLASH 1
#define ADC_TRIM_EXT_FLASH 2
#define ADC_TRIM_NO_TRIM   3
#define ADC_TRIM_MODE ADC_TRIM_NO_TRIM//ADC_TRIM_SIP_FLASH // define adc trim mode
#endif

/**
 *******************************************************************************************************************************************
 * @brief          Calculate the average value of the array items excluding the maximum & minimum. Round in & round out will also be executed.
 * @param[inout]   *numbers    Pointer of array.
 * @return         Average value.
 *******************************************************************************************************************************************
 */
static uint16_t AvgArray(uint16_t* numbers);

#if (defined BX_BATTERY_MONITOR) && (BX_BATTERY_MONITOR == 1)
/**
 *******************************************************************************************************************************************
 * @brief          Set current battery voltage.
 *******************************************************************************************************************************************
 */
void app_set_cur_bat(void);

/**
 *******************************************************************************************************************************************
 * @brief          Get current battery voltage.
 * @return         Current battery voltage.
 *******************************************************************************************************************************************
 */
uint16_t app_get_cur_bat(void);
/**
 *******************************************************************************************************************************************
 * @brief          Adjust system registers according to current battery voltage.
 *******************************************************************************************************************************************
 */
void adc_sys_adjust_bat(void);
#endif

/**
 *******************************************************************************************************************************************
 * @brief          ADC comparator function for qsort.
 * @param[in]      *a    Pointer of item to be compared.
 * @param[out]     *b    Pointer of item to be compared.
 * @return         Compared result.
 *******************************************************************************************************************************************
 */
//static int adc_comparator(const void *a, const void *b);

/**
 *******************************************************************************************************************************************
 * @brief          Read trim RO value function.
 *******************************************************************************************************************************************
 */
void app_adc_readRO(void);

/**
 *******************************************************************************************************************************************
 * @brief          ADC comparator function for qsort.
 * @param[in]      *a    Pointer of item to be compared.
 * @param[out]     *b    Pointer of item to be compared. 
 * @return         Compared result.
 *******************************************************************************************************************************************
 */
static int adc_comparator(const void *a, const void *b);

/**
 *******************************************************************************************************************************************
 * @brief          Function getting battery monitor raw value.
 * @param[in]      *adc_p    Pointer of adc instance.
 * @return         Battery raw value.
 *******************************************************************************************************************************************
 */
static uint16_t app_adc_battery_raw(app_adc_inst_t *adc_p);

#if ADC_GPADC_RO_TRIM
/**
 *******************************************************************************************************************************************
 * @brief          Function getting gpadc single end raw value. 
 * @param[in]      *adc_p    Pointer of adc instance.
 * @return         Single end gpadc raw value.
 *******************************************************************************************************************************************
 */
static uint16_t app_adc_gpadc_single_end_raw(uint8_t channel);
/**
 *******************************************************************************************************************************************
 * @brief          Function judging gpadc bonding trim RO bits.
 * @param[in]      channel  ADC channel.
 * @return         Battery raw value.
 *******************************************************************************************************************************************
 */
static uint8_t app_adc_gpadc_bonding_RO_bit(uint32_t channel);
#endif

/**
 *******************************************************************************************************************************************
 * @brief          Calculate RO value under battery=3.9V.
 *******************************************************************************************************************************************
 */
void app_adc_RO_sim_3V9(void);
/**
 *******************************************************************************************************************************************
 * @brief          Calculate RO value under any circumstance.
 *******************************************************************************************************************************************
 */
void app_adc_util_init(void);
/**
 *******************************************************************************************************************************************
 * @brief          Convert battery adc value to voltage.
 * @param[in]      bat_val Battery value.
 * @return         Voltage value(in mini volt).
 *******************************************************************************************************************************************
 */
static uint32_t app_adc_battery_volt(uint32_t bat_val);
/**
 *******************************************************************************************************************************************
 * @brief          Get current battery voltage.
 * @return         Current battery voltage value(in mini volt).
 *******************************************************************************************************************************************
 */
uint32_t app_adc_battery(void);

#if ADC_TEMP_SENSOR_EN
/**
 *******************************************************************************************************************************************
 * @brief          Function getting temperature sensor raw value.
 * @param[in]      *adc_p    Pointer of adc instance.
 * @return         Temperature sensor raw value.
 *******************************************************************************************************************************************
 */
static uint16_t app_adc_tempSensor_raw(app_adc_inst_t *adc_p);

/**
 *******************************************************************************************************************************************
 * @brief          Get current temperature sensor.
 * @return         Current temperature sensor value.
 *******************************************************************************************************************************************
 */
int16_t app_adc_tempSensor(void);
#endif
#if (defined BX_TEMP_SENSOR) && (BX_TEMP_SENSOR == 1)  
/**
 *******************************************************************************************************************************************
 * @brief          Set current temperature sensor.
 *******************************************************************************************************************************************
 */
void app_set_cur_temp(void);
/**
 *******************************************************************************************************************************************
 * @brief          Get current temperature sensor.
 * @return         Current temperature sensor value.
 *******************************************************************************************************************************************
 */
uint16_t app_get_cur_temp(void);
/**
 *******************************************************************************************************************************************
 * @brief          Adjust system registers according to current temperature sensor.
 *******************************************************************************************************************************************
 */
void adc_sys_adjust_temp(void);
#endif

#if ADC_GPADC_SINGLE_END_MODE_EN
/**
 *******************************************************************************************************************************************
 * @brief          Convert single end adc value to voltage.
 * @param[in]      gpadc_val Gpadc value.
 * @return         Voltage value(in mini volt).
 *******************************************************************************************************************************************
 */
static uint32_t app_adc_gpadc_single_end_volt(uint32_t gpadc_val);
/**
 *******************************************************************************************************************************************
 * @brief          Get current single end gpadc voltage.
 * @param[in]      channel Channel to sample. 
 * @return         Current gpadc voltage value(in mini volt).
 *******************************************************************************************************************************************
 */
uint32_t app_adc_gpadc_single_end(uint8_t channel);
#endif
#if ADC_GPADC_DIFFERENTIAL_MODE_EN
/**
 *******************************************************************************************************************************************
 * @brief          Get current differential gpadc voltage.
 * @param[in]      channel Channel to sample. 
 * @return         Current gpadc voltage value(in mini volt).
 *******************************************************************************************************************************************
 */
uint32_t app_adc_gpadc_differential(uint8_t channel);
#endif
