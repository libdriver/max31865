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
#include <getopt.h>
#include <stdlib.h>

/**
 * @brief     max31865 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t max31865(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"resistor", required_argument, NULL, 1},
        {"type", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {"wire", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    float r = 430.0f;
    max31865_resistor_t chip_type = MAX31865_RESISTOR_100PT;
    max31865_wire_t wire = MAX31865_WIRE_4;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* resistor */
            case 1 :
            {
                /* set the resistor*/
                r = atof(optarg);
                
                break;
            }
            
            /* type */
            case 2 :
            {
                /* set the chip type */
                if (strcmp("100", optarg) == 0)
                {
                    chip_type = MAX31865_RESISTOR_100PT;
                }
                else if (strcmp("1000", optarg) == 0)
                {
                    chip_type = MAX31865_RESISTOR_1000PT;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* running times */
            case 3 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* wire */
            case 4 :
            {
                /* set the mode */
                if (strcmp("2", optarg) == 0)
                {
                    wire = MAX31865_WIRE_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    wire = MAX31865_WIRE_3;
                }
                else if (strcmp("4", optarg) == 0)
                {
                    wire = MAX31865_WIRE_4;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run reg test */
        res = max31865_register_test();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* run read test */
        res = max31865_read_test(wire, chip_type, r, times);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temp;
        
        /* basic init */
        res = max31865_basic_init(wire, chip_type, r);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = max31865_basic_read((float *)&temp);
            if (res != 0)
            {
                (void)max31865_basic_deinit();
                
                return 1;
            }
            
            /* output */
            max31865_interface_debug_print("max31865: %d/%d.\n", i + 1, times);
            max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
            
            /* delay 1000ms */
            max31865_interface_delay_ms(1000);
        }
        
        /* basic deinit */
        (void)max31865_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        float temp;
        
        /* shot init */
        res = max31865_shot_init(wire, chip_type, r);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* read data */
            res = max31865_shot_read((float *)&temp);
            if (res != 0)
            {
                (void)max31865_shot_deinit();
                
                return 1;
            }
            
            /* output */
            max31865_interface_debug_print("max31865: %d/%d.\n", i + 1, times);
            max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
            
            /* delay 1000ms */
            max31865_interface_delay_ms(1000);
        }
        
        /* shot deinit */
        (void)max31865_shot_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        max31865_interface_debug_print("Usage:\n");
        max31865_interface_debug_print("  max31865 (-i | --information)\n");
        max31865_interface_debug_print("  max31865 (-h | --help)\n");
        max31865_interface_debug_print("  max31865 (-p | --port)\n");
        max31865_interface_debug_print("  max31865 (-t reg | --test=reg)\n");
        max31865_interface_debug_print("  max31865 (-t read | --test=read) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>]\n");
        max31865_interface_debug_print("           [--resistor=<r>] [--times=<num>]\n");
        max31865_interface_debug_print("  max31865 (-e read | --example=read) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>]\n");
        max31865_interface_debug_print("           [--resistor=<r>] [--times=<num>]\n");
        max31865_interface_debug_print("  max31865 (-e shot | --example=shot) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>]\n");
        max31865_interface_debug_print("           [--resistor=<r>] [--times=<num>]\n");
        max31865_interface_debug_print("\n");
        max31865_interface_debug_print("Options:\n");
        max31865_interface_debug_print("  -e <fifo>, --example=<fifo>    Run the driver example.\n");
        max31865_interface_debug_print("  -h, --help                     Show the help.\n");
        max31865_interface_debug_print("  -i, --information              Show the chip information.\n");
        max31865_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        max31865_interface_debug_print("      --resistor=<r>             Set the reference resistor.([default: 430.0f])\n");
        max31865_interface_debug_print("  -t <reg | fifo>, --test=<reg | fifo>\n");
        max31865_interface_debug_print("                                 Run the driver test.\n");
        max31865_interface_debug_print("      --type=<100 | 1000>        Set the PT resistor type.([default: 100])\n");
        max31865_interface_debug_print("      --times=<num>              Set the running times.([default: 3])\n");
        max31865_interface_debug_print("      --wire=<2 | 3 | 4>         Set the PT resistor wire.([default: 4])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        max31865_info_t info;
        
        /* print max31865 info */
        max31865_info(&info);
        max31865_interface_debug_print("max31865: chip is %s.\n", info.chip_name);
        max31865_interface_debug_print("max31865: manufacturer is %s.\n", info.manufacturer_name);
        max31865_interface_debug_print("max31865: interface is %s.\n", info.interface);
        max31865_interface_debug_print("max31865: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max31865_interface_debug_print("max31865: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max31865_interface_debug_print("max31865: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max31865_interface_debug_print("max31865: max current is %0.2fmA.\n", info.max_current_ma);
        max31865_interface_debug_print("max31865: max temperature is %0.1fC.\n", info.temperature_max);
        max31865_interface_debug_print("max31865: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        max31865_interface_debug_print("max31865: SCK connected to GPIO11(BCM).\n");
        max31865_interface_debug_print("max31865: MISO connected to GPIO9(BCM).\n");
        max31865_interface_debug_print("max31865: MOSI connected to GPIO10(BCM).\n");
        max31865_interface_debug_print("max31865: CS connected to GPIO8(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = max31865(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        max31865_interface_debug_print("max31865: run failed.\n");
    }
    else if (res == 5)
    {
        max31865_interface_debug_print("max31865: param is invalid.\n");
    }
    else
    {
        max31865_interface_debug_print("max31865: unknown status code.\n");
    }

    return 0;
}
