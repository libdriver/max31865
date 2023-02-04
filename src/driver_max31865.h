/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_max31865.h
 * @brief     driver max31865 header file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-02-22
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/22  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/10/17  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MAX31865_H
#define DRIVER_MAX31865_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max31865_driver max31865 driver function
 * @brief    max31865 driver modules
 * @{
 */

/**
 * @addtogroup max31865_base_driver
 * @{
 */

/**
 * @brief max31865 bool enumeration definition
 */
typedef enum
{
    MAX31865_BOOL_FALSE = 0x00,        /**< disable function */
    MAX31865_BOOL_TRUE  = 0x01,        /**< enable function */
} max31865_bool_t;

/**
 * @brief max31865 wire type enumeration definition
 */
typedef enum
{ 
    MAX31865_WIRE_2 = 0x00,        /**< 2 wire mode */
    MAX31865_WIRE_3 = 0x01,        /**< 3 wire mode */
    MAX31865_WIRE_4 = 0x00,        /**< 4 wire mode */
} max31865_wire_t;

/**
 * @brief max31865 resistor type enumeration definition
 */
typedef enum
{ 
    MAX31865_RESISTOR_100PT  = 0x00,        /**< 100pt resistor type */
    MAX31865_RESISTOR_1000PT = 0x01,        /**< 1000pt resistor type */
} max31865_resistor_t;

/**
 * @brief max31865 filter select enumeration definition
 */
typedef enum
{ 
    MAX31865_FILTER_SELECT_60HZ = 0x00,        /**< 60Hz noise filter */
    MAX31865_FILTER_SELECT_50HZ = 0x01,        /**< 50Hz noise filter */
} max31865_filter_select_t;

/**
 * @brief max31865 fault detection cycle control enumeration definition
 */
typedef enum
{ 
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_NO_ACTION            = 0x00,        /**< no action */
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_AUTOMATIC_DELAY      = 0x01,        /**< automatic delay */
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_MANUAL_DELAY_CYCLE_1 = 0x02,        /**< manual delay cycle 1 */
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_MANUAL_DELAY_CYCLE_2 = 0x03,        /**< manual delay cycle 2 */
} max31865_fault_detection_cycle_control_t;

/**
 * @brief max31865 fault detection cycle control status enumeration definition
 */
typedef enum
{ 
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_STATUS_FINISHED                     = 0x00,        /**< finished status */
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_STATUS_AUTOMATIC_RUNNING            = 0x01,        /**< automatic running status */
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_STATUS_MANUAL_DELAY_CYCLE_1_RUNNING = 0x02,        /**< manual delay cycle 1 running status */
    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_STATUS_MANUAL_DELAY_CYCLE_2_RUNNING = 0x03,        /**< manual delay cycle 2 running status */
} max31865_fault_detection_cycle_control_status_t;

/**
 * @brief max31865 fault status enumeration definition
 */
typedef enum
{
    MAX31865_FAULT_STATUS_RTD_HIGH_THRESHOLD         = (1 << 7),        /**< rtd high threshold */
    MAX31865_FAULT_STATUS_RTD_LOW_THRESHOLD          = (1 << 6),        /**< rtd low threshold */
    MAX31865_FAULT_STATUS_REFIN_MORE_THAN_0P85_VBIAS = (1 << 5),        /**< ref more than 0.85 vbias */
    MAX31865_FAULT_STATUS_REFIN_LESS_THAN_0P85_VBIAS = (1 << 4),        /**< ref less than 0.85 vbias */
    MAX31865_FAULT_STATUS_RTDIN_LESS_THAN_0P85_VBIAS = (1 << 3),        /**< rtdin less than 0.85 vbias */
    MAX31865_FAULT_STATUS_OVER_UNDER_VOLTAGE         = (1 << 2),        /**< over under voltage */
} max31865_fault_status_t;

/**
 * @brief max31865 handle structure definition
 */
typedef struct max31865_handle_s
{
    uint8_t (*spi_init)(void);                                            /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                          /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to a spi_write function address */
    void (*delay_ms)(uint32_t ms);                                        /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                      /**< point to a debug_print function address */
    uint8_t inited;                                                       /**< inited flag */
    uint8_t resistor;                                                     /**< resistor type */
    float ref_resistor;                                                   /**< reference resistor value */
} max31865_handle_t;

/**
 * @brief max31865 information structure definition
 */
typedef struct max31865_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} max31865_info_t;

/**
 * @}
 */

/**
 * @defgroup max31865_link_driver max31865 link driver function
 * @brief    max31865 link driver modules
 * @ingroup  max31865_driver
 * @{
 */

/**
 * @brief     initialize max31865_handle_t structure
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] STRUCTURE is max31865_handle_t
 * @note      none
 */
#define DRIVER_MAX31865_LINK_INIT(HANDLE, STRUCTURE)   memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_MAX31865_LINK_SPI_INIT(HANDLE, FUC)    (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_MAX31865_LINK_SPI_DEINIT(HANDLE, FUC)  (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_MAX31865_LINK_SPI_READ(HANDLE, FUC)    (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_MAX31865_LINK_SPI_WRITE(HANDLE, FUC)   (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MAX31865_LINK_DELAY_MS(HANDLE, FUC)    (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a max31865 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MAX31865_LINK_DEBUG_PRINT(HANDLE, FUC) (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup max31865_base_driver max31865 base driver function
 * @brief    max31865 base driver modules
 * @ingroup  max31865_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a max31865 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max31865_info(max31865_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a max31865 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t max31865_init(max31865_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a max31865 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t max31865_deinit(max31865_handle_t *handle);

/**
 * @brief      read data once
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 single read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 find rtd fault
 * @note       none
 */
uint8_t max31865_single_read(max31865_handle_t *handle, uint16_t *raw, float *temp);

/**
 * @brief     start reading
 * @param[in] *handle points to a max31865 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_start_continuous_read(max31865_handle_t *handle);

/**
 * @brief     stop reading
 * @param[in] *handle points to a max31865 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_stop_continuous_read(max31865_handle_t *handle);

/**
 * @brief      read data continuously
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 continuous read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 find rtd fault
 * @note       none
 */
uint8_t max31865_continuous_read(max31865_handle_t *handle, uint16_t *raw, float *temp);

/**
 * @brief     set the filter type
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] filter is the filter type and it can be 50Hz or 60Hz
 * @return    status code
 *            - 0 success
 *            - 1 set filter select failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_filter_select(max31865_handle_t *handle, max31865_filter_select_t filter);

/**
 * @brief      get the filter type
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *filter points to a filter type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get filter select failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_filter_select(max31865_handle_t *handle, max31865_filter_select_t *filter);

/**
 * @brief     set the pt resistor wire
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] wire is the pt resistor wire and it can be 2, 3 or 4
 * @return    status code
 *            - 0 success
 *            - 1 set wire failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_wire(max31865_handle_t *handle, max31865_wire_t wire);

/**
 * @brief      get the pt resistor wire
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *wire points to a pt resistor wire type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get wire failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_wire(max31865_handle_t *handle, max31865_wire_t *wire);

/**
 * @brief     set the reference resistor
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] value is the reference resistor value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_reference_resistor(max31865_handle_t *handle, float value);

/**
 * @brief      get the reference resistor
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *value points to a reference resistor buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_reference_resistor(max31865_handle_t *handle, float *value);

/**
 * @brief     set the pt resistor 
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] resistor is the pt resistor type value
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_resistor(max31865_handle_t *handle, max31865_resistor_t resistor);

/**
 * @brief      get the pt resistor 
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *resistor points to a pt resistor type value buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_resistor(max31865_handle_t *handle, max31865_resistor_t *resistor);

/**
 * @brief     set the chip vbias
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set vbias failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_vbias(max31865_handle_t *handle, max31865_bool_t enable);

/**
 * @brief      get the chip vbias
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get vbias failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_vbias(max31865_handle_t *handle, max31865_bool_t *enable);

/**
 * @brief     clear all fault status
 * @param[in] *handle points to a max31865 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear fault status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_clear_fault_status(max31865_handle_t *handle);

/**
 * @brief      get the fault status
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *status points to a fault status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fault status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_fault_status(max31865_handle_t *handle, uint8_t *status);

/**
 * @brief     set the high fault threshold
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] threshold is the high fault threshold
 * @return    status code
 *            - 0 success
 *            - 1 set high fault threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_high_fault_threshold(max31865_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the high fault threshold
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *threshold points to a high fault threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get high fault threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_high_fault_threshold(max31865_handle_t *handle, uint16_t *threshold);

/**
 * @brief     set the low fault threshold
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] threshold is the low fault threshold
 * @return    status code
 *            - 0 success
 *            - 1 set low fault threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_low_fault_threshold(max31865_handle_t *handle, uint16_t threshold);

/**
 * @brief      get the low fault threshold
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *threshold points to a low fault threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get low fault threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_low_fault_threshold(max31865_handle_t *handle, uint16_t *threshold);

/**
 * @brief     set the fault detection cycle control
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] control is the fault detection cycle
 * @return    status code
 *            - 0 success
 *            - 1 set fault detection cycle control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_fault_detection_cycle_control(max31865_handle_t *handle, max31865_fault_detection_cycle_control_t control);

/**
 * @brief      get the fault detection cycle control
 * @param[in]  *handle points to a max31865 handle structure
 * @param[out] *status points to a fault detection cycle buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fault detection cycle control failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_fault_detection_cycle_control(max31865_handle_t *handle, max31865_fault_detection_cycle_control_status_t *status);

/**
 * @}
 */

/**
 * @defgroup max31865_extern_driver max31865 extern driver function
 * @brief    max31865 extern driver modules
 * @ingroup  max31865_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to a max31865 handle structure
 * @param[in] reg is the register address
 * @param[in] value is the register value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31865_set_reg(max31865_handle_t *handle, uint8_t reg, uint8_t value);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to a max31865 handle structure
 * @param[in]  reg is the register address
 * @param[out] *value points to a register value buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31865_get_reg(max31865_handle_t *handle, uint8_t reg, uint8_t *value);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
