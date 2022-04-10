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
 * @file      driver_max31865_basic.h
 * @brief     driver max31865 basic header file
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

#ifndef DRIVER_MAX31865_BASIC_H
#define DRIVER_MAX31865_BASIC_H

#include "driver_max31865_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max31865_example_driver max31865 example driver function
 * @brief    max31865 example driver modules
 * @ingroup  max31865_driver
 * @{
 */

/**
 * @brief max31865 basic example default definition
 */
#define MAX31865_BASIC_DEFAULT_FILTER_SELECT                    MAX31865_FILTER_SELECT_50HZ                                   /**< filter select 50Hz */
#define MAX31865_BASIC_DEFAULT_FAULT_DETECTION_CYCLE_CONTROL    MAX31865_FAULT_DETECTION_CYCLE_CONTROL_AUTOMATIC_DELAY        /**< automatic delay */
#define MAX31865_BASIC_DEFAULT_HIGH_FAULT_THRESHOLD             0xFFFEU                                                       /**< high fault threshold */
#define MAX31865_BASIC_DEFAULT_LOW_FAULT_THRESHOLD              0x0000U                                                       /**< low fault threshold */

/**
 * @brief     basic example init
 * @param[in] wire is the pt resistor wire
 * @param[in] type is the pt resistor type
 * @param[in] ref_resistor is the reference resistor value
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max31865_basic_init(max31865_wire_t wire, max31865_resistor_t type, float ref_resistor);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max31865_basic_deinit(void);

/**
 * @brief      basic example read the temperature
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31865_basic_read(float *temp);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
