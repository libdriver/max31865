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
 * @file      driver_max31865_basic.c
 * @brief     driver max31865 basic source file
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

#include "driver_max31865_basic.h"

static max31865_handle_t gs_handle;        /**< max31865 handle */

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
uint8_t max31865_basic_init(max31865_wire_t wire, max31865_resistor_t type, float ref_resistor)
{
    uint8_t res;
    
    /* link function */
    DRIVER_MAX31865_LINK_INIT(&gs_handle, max31865_handle_t);
    DRIVER_MAX31865_LINK_SPI_INIT(&gs_handle, max31865_interface_spi_init);
    DRIVER_MAX31865_LINK_SPI_DEINIT(&gs_handle, max31865_interface_spi_deinit);
    DRIVER_MAX31865_LINK_SPI_READ(&gs_handle, max31865_interface_spi_read);
    DRIVER_MAX31865_LINK_SPI_WRITE(&gs_handle, max31865_interface_spi_write);
    DRIVER_MAX31865_LINK_DELAY_MS(&gs_handle, max31865_interface_delay_ms);
    DRIVER_MAX31865_LINK_DEBUG_PRINT(&gs_handle, max31865_interface_debug_print);
    
    /* max31865 init */
    res = max31865_init(&gs_handle);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: init failed.\n");
        
        return 1;
    }
    
    /* set filter */
    res = max31865_set_filter_select(&gs_handle, MAX31865_BASIC_DEFAULT_FILTER_SELECT);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set filter select failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set wire type */
    res = max31865_set_wire(&gs_handle, wire);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set PT resistor type */
    res = max31865_set_resistor(&gs_handle, type);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set resistor type failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set reference resistor */
    res = max31865_set_reference_resistor(&gs_handle, ref_resistor);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set reference resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fault detection cycle control */
    res = max31865_set_fault_detection_cycle_control(&gs_handle, MAX31865_BASIC_DEFAULT_FAULT_DETECTION_CYCLE_CONTROL);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high fault threshold */
    res = max31865_set_high_fault_threshold(&gs_handle, MAX31865_BASIC_DEFAULT_HIGH_FAULT_THRESHOLD);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set high fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set low fault threshold */
    res = max31865_set_low_fault_threshold(&gs_handle, MAX31865_BASIC_DEFAULT_LOW_FAULT_THRESHOLD);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set low fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start continuous read */
    res = max31865_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: start continuous read failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read the temperature
 * @param[out] *temp points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31865_basic_read(float *temp)
{
    uint16_t raw;
    
    /* continuous read */
    if (max31865_continuous_read(&gs_handle, (uint16_t *)&raw, temp) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max31865_basic_deinit(void)
{
    uint8_t res;
    
    /* stop continuous read */
    res = max31865_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    /* deinit */
    if (max31865_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
