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
 * @file      driver_max31865_register_test.c
 * @brief     driver max31865 register test source file
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
 
#include "driver_max31865_register_test.h"
#include <math.h>
#include <stdlib.h>

static max31865_handle_t gs_handle;        /**< max31865 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t max31865_register_test(void)
{
    uint8_t res;
    float resistor_in;
    float resistor_out;
    uint16_t threshold_in;
    uint16_t threshold_out;
    max31865_info_t info;
    max31865_filter_select_t filter;
    max31865_wire_t wire;
    max31865_resistor_t resistor;
    max31865_bool_t enable;
    max31865_fault_detection_cycle_control_status_t status;
    
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
    
    /* start register test */
    max31865_interface_debug_print("max31865: start register test.\n");
    
    /* max31865_set_filter_select/max31865_get_filter_select test */
    max31865_interface_debug_print("max31865: max31865_set_filter_select/max31865_get_filter_select test.\n");
    
    /* set 60Hz */
    res = max31865_set_filter_select(&gs_handle, MAX31865_FILTER_SELECT_60HZ);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set filter select failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set filter select 60Hz.\n");
    res = max31865_get_filter_select(&gs_handle, &filter);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get filter select failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check filter select %s.\n", filter == MAX31865_FILTER_SELECT_60HZ ? "ok" : "error");
    
    /* set 50Hz */
    res = max31865_set_filter_select(&gs_handle, MAX31865_FILTER_SELECT_50HZ);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set filter select failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set filter select 50Hz.\n");
    res = max31865_get_filter_select(&gs_handle, &filter);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get filter select failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check filter select %s.\n", filter == MAX31865_FILTER_SELECT_50HZ ? "ok" : "error");
    
    /* max31865_set_wire/max31865_get_wire test */
    max31865_interface_debug_print("max31865: max31865_set_wire/max31865_get_wire test.\n");
    
    /* set wire 2 */
    res = max31865_set_wire(&gs_handle, MAX31865_WIRE_2);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set 2 wire.\n");
    res = max31865_get_wire(&gs_handle, &wire);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check wire %s.\n", wire == MAX31865_WIRE_2 ? "ok" : "error");
    
    /* set wire 3 */
    res = max31865_set_wire(&gs_handle, MAX31865_WIRE_3);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set 3 wire.\n");
    res = max31865_get_wire(&gs_handle, &wire);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check wire %s.\n", wire == MAX31865_WIRE_3 ? "ok" : "error");
    
    /* set wire 4 */
    res = max31865_set_wire(&gs_handle, MAX31865_WIRE_4);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set 4 wire.\n");
    res = max31865_get_wire(&gs_handle, &wire);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get wire failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check wire %s.\n", wire == MAX31865_WIRE_4 ? "ok" : "error");
    
    /* max31865_set_resistor/max31865_get_resistor test */
    max31865_interface_debug_print("max31865: max31865_set_resistor/max31865_get_resistor test.\n");
    
    /* set 100PT */
    res = max31865_set_resistor(&gs_handle, MAX31865_RESISTOR_100PT);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set 100 pt resistor.\n");
    res = max31865_get_resistor(&gs_handle, &resistor);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check resistor %s.\n", resistor == MAX31865_RESISTOR_100PT ? "ok" : "error");
    
    /* set 1000PT */
    res = max31865_set_resistor(&gs_handle, MAX31865_RESISTOR_1000PT);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set 1000 pt resistor.\n");
    res = max31865_get_resistor(&gs_handle, &resistor);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check resistor %s.\n", resistor == MAX31865_RESISTOR_1000PT ? "ok" : "error");
    
    /* max31865_set_reference_resistor/max31865_get_reference_resistor test */
    max31865_interface_debug_print("max31865: max31865_set_reference_resistor/max31865_get_reference_resistor test.\n");
    resistor_in = (float)(rand()%65536)/100.0f;
    res = max31865_set_reference_resistor(&gs_handle, resistor_in);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set reference resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set reference resistor %f.\n", resistor_in);
    res = max31865_get_reference_resistor(&gs_handle, (float *)&resistor_out);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get reference resistor failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check reference resistor %s.\n", fabsf(resistor_out - resistor_in) < 0.000001f ? "ok" : "error");
    
    /* max31865_set_vbias/max31865_get_vbias test */
    max31865_interface_debug_print("max31865: max31865_set_vbias/max31865_get_vbias test.\n");
    
    /* enable */
    res = max31865_set_vbias(&gs_handle, MAX31865_BOOL_TRUE);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set vbias failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: enable vbias.\n");
    res = max31865_get_vbias(&gs_handle, &enable);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get vbias failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check vbias %s.\n", enable == MAX31865_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = max31865_set_vbias(&gs_handle, MAX31865_BOOL_FALSE);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set vbias failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: disable vbias.\n");
    res = max31865_get_vbias(&gs_handle, &enable);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get vbias failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check vbias %s.\n", enable == MAX31865_BOOL_FALSE ? "ok" : "error");
    
    /* max31865_set_fault_detection_cycle_control test */
    max31865_interface_debug_print("max31865: max31865_set_fault_detection_cycle_control test.\n");
    
    /* no action */
    res = max31865_set_fault_detection_cycle_control(&gs_handle, MAX31865_FAULT_DETECTION_CYCLE_CONTROL_NO_ACTION);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check fault detection cycle control %s.\n", res==0?"ok":"error");
    
    /* automatic delay */
    res = max31865_set_fault_detection_cycle_control(&gs_handle, MAX31865_FAULT_DETECTION_CYCLE_CONTROL_AUTOMATIC_DELAY);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check fault detection cycle control %s.\n", res==0?"ok":"error");
    
    /* manual delay cycle 1 */
    res = max31865_set_fault_detection_cycle_control(&gs_handle, MAX31865_FAULT_DETECTION_CYCLE_CONTROL_MANUAL_DELAY_CYCLE_1);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check fault detection cycle control %s.\n", res==0?"ok":"error");
    
    /* manual delay cycle 2 */
    res = max31865_set_fault_detection_cycle_control(&gs_handle, MAX31865_FAULT_DETECTION_CYCLE_CONTROL_MANUAL_DELAY_CYCLE_2);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check fault detection cycle control %s.\n", res==0?"ok":"error");
    
    /* max31865_get_fault_detection_cycle_control test */
    max31865_interface_debug_print("max31865: max31865_get_fault_detection_cycle_control test.\n");
    res = max31865_get_fault_detection_cycle_control(&gs_handle, &status);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get fault detection cycle control failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check get fault detection cycle control %s 0x%02X.\n", res==0?"ok":"error", status);
    
    /* max31865_clear_fault_status test */
    max31865_interface_debug_print("max31865: max31865_clear_fault_status test.\n");
    res = max31865_clear_fault_status(&gs_handle);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: clear fault status failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check clear fault status %s.\n", res == 0 ? "ok" : "error");
    
    /* max31865_set_high_fault_threshold/max31865_get_high_fault_threshold test */
    max31865_interface_debug_print("max31865: max31865_set_high_fault_threshold/max31865_get_high_fault_threshold test.\n");
    threshold_in = rand() % 32768;
    res = max31865_set_high_fault_threshold(&gs_handle, threshold_in);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set high fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set high fault threshold %d.\n", threshold_in);
    res = max31865_get_high_fault_threshold(&gs_handle, (uint16_t *)&threshold_out);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get high fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check high fault threshold %s.\n", threshold_out == threshold_in ? "ok" : "error");
    
    /* max31865_set_low_fault_threshold/max31865_get_low_fault_threshold test */
    max31865_interface_debug_print("max31865: max31865_set_low_fault_threshold/max31865_get_low_fault_threshold test.\n");
    threshold_in = rand() % 32768;
    res = max31865_set_low_fault_threshold(&gs_handle, threshold_in);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: set low fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: set low fault threshold %d.\n", threshold_in);
    res = max31865_get_low_fault_threshold(&gs_handle, (uint16_t *)&threshold_out);
    if (res != 0)
    {
        max31865_interface_debug_print("max31865: get low fault threshold failed.\n");
        (void)max31865_deinit(&gs_handle);
        
        return 1;
    }
    max31865_interface_debug_print("max31865: check low fault threshold %s.\n", threshold_out == threshold_in ? "ok" : "error");
    
    /* finish register test */
    max31865_interface_debug_print("max31865: finish register test.\n");
    (void)max31865_deinit(&gs_handle);
    
    return 0;
}
