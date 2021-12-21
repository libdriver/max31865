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
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-02-22
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/22  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_max31865_read_test.h"
#include "driver_max31865_register_test.h"
#include "driver_max31865_shot.h"
#include "driver_max31865_basic.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
uint16_t g_len;            /**< uart buffer length */

/**
 * @brief     max31865 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t max31865(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            max31865_info_t info;
            
            /* print max31865 info */
            max31865_info(&info);
            max31865_interface_debug_print("max31865: chip is %s.\n", info.chip_name);
            max31865_interface_debug_print("max31865: manufacturer is %s.\n", info.manufacturer_name);
            max31865_interface_debug_print("max31865: interface is %s.\n", info.interface);
            max31865_interface_debug_print("max31865: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            max31865_interface_debug_print("max31865: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            max31865_interface_debug_print("max31865: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            max31865_interface_debug_print("max31865: max current is %0.2fmA.\n", info.max_current_ma);
            max31865_interface_debug_print("max31865: max temperature is %0.1fC.\n", info.temperature_max);
            max31865_interface_debug_print("max31865: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            max31865_interface_debug_print("max31865: SCK connected to GPIOA PIN5.\n");
            max31865_interface_debug_print("max31865: MISO connected to GPIOA PIN6.\n");
            max31865_interface_debug_print("max31865: MOSI connected to GPIOA PIN7.\n");
            max31865_interface_debug_print("max31865: CS connected to GPIOA PIN4.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show max31865 help */
            
            help:
            
            max31865_interface_debug_print("max31865 -i\n\tshow max31865 chip and driver information.\n");
            max31865_interface_debug_print("max31865 -h\n\tshow max31865 help.\n");
            max31865_interface_debug_print("max31865 -p\n\tshow max31865 pin connections of the current board.\n");
            max31865_interface_debug_print("max31865 -t reg\n\trun max31865 register test.\n");
            max31865_interface_debug_print("max31865 -t read <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>\n\t"
                                           "run max31865 read test.times means test times.resistor means reference resistor.\n");
            max31865_interface_debug_print("max31865 -c read <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>\n\t"
                                           "run max31865 read function.times means test times.resistor means reference resistor.\n");
            max31865_interface_debug_print("max31865 -c shot <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>\n\t" 
                                           "run max31865 shot function.times means test times.resistor means reference resistor.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                volatile uint8_t res;
                
                res = max31865_register_test();
                if (res)
                {
                    return 1;
                }
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 10)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* read test */
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                max31865_wire_t wire;
                max31865_resistor_t type;
                
                if (strcmp("-wire", argv[4]) != 0)
                {
                    return 5;
                }
                if (strcmp("-type", argv[6]) != 0)
                {
                    return 5;
                }
                if (strcmp("-r", argv[8]) != 0)
                {
                    return 5;
                }
                if (strcmp("2", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_2;
                }
                else if (strcmp("3", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_3;
                }
                else if (strcmp("4", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_4;
                }
                else
                {
                    max31865_interface_debug_print("max31865: wire type is invalid.\n");
                   
                    return 5;
                }
                if (strcmp("100", argv[7]) == 0)
                {
                    type = MAX31865_RESISTOR_100PT;
                }
                else if (strcmp("1000", argv[7]) == 0)
                {
                    type =  MAX31865_RESISTOR_1000PT;
                }
                else
                {
                    max31865_interface_debug_print("max31865: resistor type is invalid.\n");
                   
                    return 5;
                }
                
                res = max31865_read_test(wire, type, atof(argv[9]), atoi(argv[3]));
                if (res)
                {
                    return 1;
                }
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* run function */
        else if (strcmp("-c", argv[1]) == 0)
        {
             /* read function */
            if (strcmp("read", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float temp;
                max31865_wire_t wire;
                max31865_resistor_t type;
                
                if (strcmp("-wire", argv[4]) != 0)
                {
                    return 5;
                }
                if (strcmp("-type", argv[6]) != 0)
                {
                    return 5;
                }
                if (strcmp("-r", argv[8]) != 0)
                {
                    return 5;
                }
                if (strcmp("2", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_2;
                }
                else if (strcmp("3", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_3;
                }
                else if (strcmp("4", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_4;
                }
                else
                {
                    max31865_interface_debug_print("max31865: wire type is invalid.\n");
                   
                    return 5;
                }
                if (strcmp("100", argv[7]) == 0)
                {
                    type = MAX31865_RESISTOR_100PT;
                }
                else if (strcmp("1000", argv[7]) == 0)
                {
                    type =  MAX31865_RESISTOR_1000PT;
                }
                else
                {
                    max31865_interface_debug_print("max31865: resistor type is invalid.\n");
                   
                    return 5;
                }
                
                res = max31865_basic_init(wire, type, atof(argv[9]));
                if (res)
                {
                    return 1;
                }
                times = atoi(argv[3]);
                for (i=0; i<times; i++)
                {
                    res = max31865_basic_read((float *)&temp);
                    if (res)
                    {
                        max31865_basic_deinit();
                        
                        return 1;
                    }
                    max31865_interface_debug_print("max31865: %d/%d.\n", i+1, times);
                    max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
                    max31865_interface_delay_ms(1000);
                }
                max31865_basic_deinit();
                
                return 0;
            }
            /* shot function */
            else if (strcmp("shot", argv[2]) == 0)
            {
                volatile uint8_t res;
                volatile uint32_t i, times;
                volatile float temp;
                max31865_wire_t wire;
                max31865_resistor_t type;
                
                if (strcmp("-wire", argv[4]) != 0)
                {
                    return 5;
                }
                if (strcmp("-type", argv[6]) != 0)
                {
                    return 5;
                }
                if (strcmp("-r", argv[8]) != 0)
                {
                    return 5;
                }
                if (strcmp("2", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_2;
                }
                else if (strcmp("3", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_3;
                }
                else if (strcmp("4", argv[5]) == 0)
                {
                    wire = MAX31865_WIRE_4;
                }
                else
                {
                    max31865_interface_debug_print("max31865: wire type is invalid.\n");
                   
                    return 5;
                }
                if (strcmp("100", argv[7]) == 0)
                {
                    type = MAX31865_RESISTOR_100PT;
                }
                else if (strcmp("1000", argv[7]) == 0)
                {
                    type =  MAX31865_RESISTOR_1000PT;
                }
                else
                {
                    max31865_interface_debug_print("max31865: resistor type is invalid.\n");
                   
                    return 5;
                }
                
                res = max31865_shot_init(wire, type, atof(argv[9]));
                if (res)
                {
                    return 1;
                }
                times = atoi(argv[3]);
                for (i=0; i<times; i++)
                {
                    res = max31865_shot_read((float *)&temp);
                    if (res)
                    {
                        max31865_basic_deinit();
                        
                        return 1;
                    }
                    max31865_interface_debug_print("max31865: %d/%d.\n", i+1, times);
                    max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
                    max31865_interface_delay_ms(1000);
                }
                max31865_shot_deinit();
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    volatile uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register max31865 fuction */
    shell_init();
    shell_register("max31865", max31865);
    uart1_print("max31865: welcome to libdriver max31865.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("max31865: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("max31865: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("max31865: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("max31865: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("max31865: param is invalid.\n");
            }
            else
            {
                uart1_print("max31865: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
