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
 * @file      driver_max31865_read_test.c
 * @brief     driver max31865 read test source file
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

#include "driver_max31865_read_test.h"

static max31865_handle_t gs_handle;        /**< max31865 handle */

/**
 * @brief     read test
 * @param[in] wire is the pt resistor wire
 * @param[in] type is the pt resistor type
 * @param[in] ref_resistor is the reference resistor value
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t max31865_read_test(max31865_wire_t wire, max31865_resistor_t type, float ref_resistor, uint32_t times)
{
    uint8_t res;
    uint32_t i;
    max31865_info_t info;
    
    /* link function */
    DRIVER_MAX31865_LINK_INIT(&gs_handle, max31865_handle_t);
    DRIVER_MAX31865_LINK_SPI_INIT(&gs_handle, max31865_interface_spi_init);
    DRIVER_MAX31865_LINK_SPI_DEINIT(&gs_handle, max31865_interface_spi_deinit);
    DRIVER_MAX31865_LINK_SPI_READ(&gs_handle, max31865_interface_spi_read);
    DRIVER_MAX31865_LINK_SPI_WRITE(&gs_handle, max31865_interface_spi_write);
    DRIVER_MAX31865_LINK_DELAY_MS(&gs_handle, max31865_interface_delay_ms);
    DRIVER_MAX31865_LINK_DEBUG_PRINT(&gs_handle, max31865_interface_debug_print);
    
    /* get chip information */
    res = max31865_info(&info);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        max31865_interface_debug_print("max31865: chip is %s.\n", info.chip_name);
        max31865_interface_debug_print("max31865: manufacturer is %s.\n", info.manufacturer_name);
        max31865_interface_debug_print("max31865: interface is %s.\n", info.interface);
        max31865_interface_debug_print("max31865: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max31865_interface_debug_print("max31865: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max31865_interface_debug_print("max31865: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max31865_interface_debug_print("max31865: max current is %0.2fmA.\n", info.max_current_ma);
        max31865_interface_debug_print("max31865: max temperature is %0.1fC.\n", info.temperature_max);
        max31865_interface_debug_print("max31865: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* max31865 init */
    res = max31865_init(&gs_handle);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: init failed.\n");
       
        return 1;
    }
    
    /* set filter select 50Hz */
    res = max31865_set_filter_select(&gs_handle, MAX31865_FILTER_SELECT_50HZ);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set filter select failed.\n");
        (void)max31865_deinit(&gs_handle);       
        
        return 1;
    }
    
    /* set wire */
    res = max31865_set_wire(&gs_handle, wire);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set resistor type */
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
    
    /* set automatic delay */
    res = max31865_set_fault_detection_cycle_control(&gs_handle, MAX31865_FAULT_DETECTION_CYCLE_CONTROL_AUTOMATIC_DELAY);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set high fault threshold */
    res = max31865_set_high_fault_threshold(&gs_handle, 0xFFFEU);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set high fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set low fault threshold */
    res = max31865_set_low_fault_threshold(&gs_handle, 0x0000U);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set low fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start read test */
    max31865_interface_debug_print("max31865: start read test.\n");
    
    /* continuous read test test */
    max31865_interface_debug_print("max31865: continuous read test test.\n");
    
    /* start continuous */
    res = max31865_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: start continuous read failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < times; i++)
    {
        uint16_t raw;
        float temp;
        
        /* delay 2000 ms */
        max31865_interface_delay_ms(2000);
    
        /* read data */
        res = max31865_continuous_read(&gs_handle, (uint16_t *)&raw, (float *)&temp);
        if (res != 0)
        {
            max31865_interface_debug_print("max31865: read failed.\n");
            (void)max31865_deinit(&gs_handle);
            
            return 1;
        }
        max31865_interface_debug_print("max31865: continuous read %0.2fC.\n", temp); 
    }

    /* stop continuous read */
    res = max31865_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: stop continuous read failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }

    /* single read test */
    max31865_interface_debug_print("max31865: single read test.\n");
    for (i = 0; i < times; i++)
    {
        uint16_t raw;
        float temp;
        
        /* delay 2000 ms */
        max31865_interface_delay_ms(2000);

        /* single read */
        res = max31865_single_read(&gs_handle, (uint16_t *)&raw, (float *)&temp);
        if (res != 0)
        {
            max31865_interface_debug_print("max31865: read failed.\n");
            (void)max31865_deinit(&gs_handle);
            
            return 1;
        }
        max31865_interface_debug_print("max31865: single read %0.2fC.\n", temp);
    }

    /* finish read test */
    max31865_interface_debug_print("max31865: finish read test.\n");
    (void)max31865_deinit(&gs_handle);
    
    return 0;
}
