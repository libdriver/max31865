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
 * @file      driver_max31865.c
 * @brief     driver max31865 source file
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

#include "driver_max31865.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Maxim Integrated MAX31865"        /**< chip name */
#define MANUFACTURER_NAME         "Maxim Integrated"                 /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                               /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                               /**< chip max supply voltage */
#define MAX_CURRENT               5.75f                              /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                             /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                              /**< chip max operating temperature */
#define DRIVER_VERSION            2000                               /**< driver version */

/**
 * @brief chip register definition
 */
#define MAX31865_REG_CONFIG                0x00        /**< config register */
#define MAX31865_REG_RTD_MSB               0x01        /**< rtd msb register */
#define MAX31865_REG_RTD_LSB               0x02        /**< rtd lsb register */
#define MAX31865_REG_HIGH_FAULT_MSB        0x03        /**< high fault msb register */
#define MAX31865_REG_HIGH_FAULT_LSB        0x04        /**< high fault lsb register */
#define MAX31865_REG_LOW_FAULT_MSB         0x05        /**< low fault msb register */
#define MAX31865_REG_LOW_FAULT_LSB         0x06        /**< low fault lsb register */
#define MAX31865_REG_FAULT_STATUS          0x07        /**< fault status register */

/**
 * @brief calculate definition
 */
#define WRITE_ADDRESS_MASK        0x80             /**< spi write mask */
#define RTD_A                     3.9083e-3f       /**< rtd a */
#define RTD_B                     -5.775e-7f       /**< rtd b */

/**
 * @brief     convert the temperature
 * @param[in] rt is the raw resistor value
 * @param[in] rtd_nominal is the resistor type value
 * @param[in] ref_resistor is the reference resistor value
 * @return    converted value
 * @note      none
 */
static float a_max31865_temperature_conversion(float rt, float rtd_nominal, float ref_resistor)
{
    float z1, z2, z3, z4, temp;
    float rpoly = rt;
    
    rt /= 32768;                             /* div 32768 */
    rt *= ref_resistor;                      /* ref_resistor */
    z1 = -RTD_A;                             /* -RTD A */
    z2 = RTD_A * RTD_A - (4 * RTD_B);        /* get z2 */
    z3 = (4 * RTD_B) / rtd_nominal;          /* get z3 */
    z4 = 2 * RTD_B;                          /* get z4 */
    temp = z2 + (z3 * rt);                   /* calculate temp */
    temp = (sqrtf(temp) + z1) / z4;          /* sqrt */
    if (temp >= 0)                           /* check temp */
    {
        return temp;                         /* return temp */
    }
    rt /= rtd_nominal;                       /* nominal */
    rt *= 100;                               /* normalize to 100 ohm */
    temp = -242.02f;                         /* -242.02 */
    temp += 2.2228f * rpoly;                 /* add offset */
    rpoly *= rt;                             /* square */
    temp += 2.5859e-3f * rpoly;              /* add offset */
    rpoly *= rt;                             /* ^3 */
    temp -= 4.8260e-6f * rpoly;              /* add offset */
    rpoly *= rt;                             /* ^4 */
    temp -= 2.8183e-8f * rpoly;              /* add offset */
    rpoly *= rt;                             /* ^5 */
    temp += 1.5243e-10f * rpoly;             /* add offset */
    
    return temp;                             /* return error */
}

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
uint8_t max31865_init(max31865_handle_t *handle)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->debug_print == NULL)                                   /* check debug_print */
    {
        return 3;                                                      /* return error */
    }
    if (handle->spi_init == NULL)                                      /* check spi_init */
    {
        handle->debug_print("max31865: spi_init is null.\n");          /* spi_init is null */
       
        return 3;                                                      /* return error */
    }
    if (handle->spi_deinit == NULL)                                    /* check spi_init */
    {
        handle->debug_print("max31865: spi_deinit is null.\n");        /* spi_deinit is null */
       
        return 3;                                                      /* return error */
    }
    if (handle->spi_read == NULL)                                      /* check spi_read */
    {
        handle->debug_print("max31865: spi_read is null.\n");          /* spi_read is null */
       
        return 3;                                                      /* return error */
    }
    if (handle->spi_write == NULL)                                     /* check spi_write */
    {
        handle->debug_print("max31865: spi_write is null.\n");         /* spi_write is null */
       
        return 3;                                                      /* return error */
    }
    if (handle->delay_ms == NULL)                                      /* check delay_ms */
    {
        handle->debug_print("max31865: delay_ms is null.\n");          /* delay_ms is null */
       
        return 3;                                                      /* return error */
    }
    
    if (handle->spi_init() != 0)                                       /* spi init */
    { 
        handle->debug_print("max31865: spi init failed.\n");           /* spi init failed */
       
        return 3;                                                      /* return error */
    }
    handle->inited = 1;                                                /* flag initialization */

    return 0;                                                          /* success return 0 */
}

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
uint8_t max31865_deinit(max31865_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                              /* read config */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                           /* read failed */
        
        return 4;                                                                                  /* return error */
    }
    prev &= ~(1 << 7);                                                                             /* clear flag */
    res = handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("max31865: write failed.\n");                                          /* write failed */
        
        return 4;                                                                                  /* return error */
    }
    if (handle->spi_deinit() != 0)                                                                 /* spi deinit */
    {
        handle->debug_print("max31865: spi deinit failed.\n");                                     /* spi deinit failed */
        
        return 1;                                                                                  /* return error */
    }         
    handle->inited = 0;                                                                            /* flag close */
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t max31865_set_filter_select(max31865_handle_t *handle, max31865_filter_select_t filter)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
       
        return 1;                                                                                   /* return error */
    }
    prev &= ~(1 << 0);                                                                              /* clear filter bit */
    prev |= filter << 0;                                                                            /* set filter */

    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_get_filter_select(max31865_handle_t *handle, max31865_filter_select_t *filter)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                     /* read failed */
       
        return 1;                                                            /* return error */
    }
    prev &= (1 << 0);                                                        /* get raw */
    *filter = (max31865_filter_select_t)(prev >> 0);                         /* get filter select */

    return 0;                                                                /* success return 0 */ 
}

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
uint8_t max31865_set_wire(max31865_handle_t *handle, max31865_wire_t wire)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
        
        return 1;                                                                                   /* return error */
    }
    prev &= ~(1 << 4);                                                                              /* clear wire bit */
    prev |= wire << 4;                                                                              /* set wire */
    
    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_get_wire(max31865_handle_t *handle, max31865_wire_t *wire)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                     /* read failed */
        
        return 1;                                                            /* return error */
    }
    prev &= 1 << 4;                                                          /* get wire bit */
    *wire = (max31865_wire_t)(prev >> 4);                                    /* get wire */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t max31865_set_reference_resistor(max31865_handle_t *handle, float value)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }
    
    handle->ref_resistor = value;        /* set reference resistor */
    
    return 0;                            /* success return 0 */
}

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
uint8_t max31865_get_reference_resistor(max31865_handle_t *handle, float *value) 
{
    if (handle == NULL)                            /* check handle */
    {
        return 2;                                  /* return error */
    }
    if (handle->inited != 1)                       /* check handle initialization */
    {
        return 3;                                  /* return error */
    }
    
    *value = (float)(handle->ref_resistor);        /* get reference resistor */
    
    return 0;                                      /* success return 0 */ 
}

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
uint8_t max31865_set_resistor(max31865_handle_t *handle, max31865_resistor_t resistor) 
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    if (handle->inited != 1)                     /* check handle initialization */
    {
        return 3;                                /* return error */
    }
    
    handle->resistor = (uint8_t)resistor;        /* set resistor */
    
    return 0;                                    /* success return 0 */
}

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
uint8_t max31865_get_resistor(max31865_handle_t *handle, max31865_resistor_t *resistor) 
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
    
    *resistor = (max31865_resistor_t)(handle->resistor);        /* get resistor */
    
    return 0;                                                   /* success return 0 */
}

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
uint8_t max31865_set_vbias(max31865_handle_t *handle, max31865_bool_t enable) 
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
        
        return 1;                                                                                   /* return error */
    }
    prev &= ~(1 << 7);                                                                              /* clear bias bit */
    prev |= enable << 7;                                                                            /* set bias enable */
    
    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_get_vbias(max31865_handle_t *handle, max31865_bool_t *enable) 
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);        /* read config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                     /* read failed */
        
        return 1;                                                            /* return error */
    }
    prev &= 1 << 7;                                                          /* get vbias bit */
    *enable = (max31865_bool_t)(prev >> 7);                                  /* get vbias */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t max31865_clear_fault_status(max31865_handle_t *handle) 
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
       
        return 1;                                                                                   /* return error */
    }
    prev &= ~(1 << 1);                                                                              /* clear bit */
    prev |= 1 << 1;                                                                                 /* set clear */

    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_get_fault_status(max31865_handle_t *handle, uint8_t *status) 
{
    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    
    return handle->spi_read(MAX31865_REG_FAULT_STATUS, status, 1);        /* read status */
}

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
uint8_t max31865_set_high_fault_threshold(max31865_handle_t *handle, uint16_t threshold) 
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                   /* check handle */
    {
        return 2;                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                              /* check handle initialization */
    {
        return 3;                                                                                         /* return error */
    }
    
    threshold = threshold << 1;                                                                           /* left shift 1 */
    buf[0] = (uint8_t)((threshold >> 8) & 0xFF);                                                          /* set msb */
    buf[1] = (uint8_t)(threshold & 0xFF);                                                                 /* set lsb */
    
    return handle->spi_write(MAX31865_REG_HIGH_FAULT_MSB | WRITE_ADDRESS_MASK, (uint8_t *)buf, 2);        /* write fault threshold */
}

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
uint8_t max31865_get_high_fault_threshold(max31865_handle_t *handle, uint16_t *threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_HIGH_FAULT_MSB, (uint8_t *)buf, 2);          /* get fault msb */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                             /* read failed */
       
        return 1;                                                                    /* return error */
    }
    *threshold = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                       /* get raw */
    *threshold = (*threshold) >> 1;                                                  /* get data */

    return 0;                                                                        /* success return 0 */
}

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
uint8_t max31865_set_low_fault_threshold(max31865_handle_t *handle, uint16_t threshold) 
{
    uint8_t buf[2];
    
    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }
    
    threshold = threshold << 1;                                                                          /* left shift 1 */
    buf[0] = (uint8_t)((threshold >> 8) & 0xFF);                                                         /* set msb */
    buf[1] = (uint8_t)(threshold & 0xFF);                                                                /* set lsb */
    
    return handle->spi_write(MAX31865_REG_LOW_FAULT_MSB | WRITE_ADDRESS_MASK, (uint8_t *)buf, 2);        /* write config */
}

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
uint8_t max31865_get_low_fault_threshold(max31865_handle_t *handle, uint16_t *threshold) 
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_LOW_FAULT_MSB, (uint8_t *)buf, 2);        /* write low fault msb */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                          /* read failed */
        
        return 1;                                                                 /* return error */
    }
    *threshold = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                    /* get raw */
    *threshold = (*threshold) >> 1;                                               /* get data */
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t max31865_set_fault_detection_cycle_control(max31865_handle_t *handle, max31865_fault_detection_cycle_control_t control) 
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
       
        return 1;                                                                                   /* return error */
    }
    prev &= ~(3 << 2);                                                                              /* clear control */
    prev |= control << 2;                                                                           /* set control */

    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_get_fault_detection_cycle_control(max31865_handle_t *handle, max31865_fault_detection_cycle_control_status_t *status)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);              /* read config */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                           /* read failed */
       
        return 1;                                                                  /* return error */
    }
    prev &= 3 << 2;                                                                /* get control bits */
    *status = (max31865_fault_detection_cycle_control_status_t)(prev >> 2);        /* get control */

    return 0;                                                                      /* success return 0 */
}

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
uint8_t max31865_single_read(max31865_handle_t *handle, uint16_t *raw, float *temp)
{
    uint8_t res, prev;
    uint8_t buf[2];
    float resistor;
    uint16_t times;
    
    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                                 /* read config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                              /* read failed */
       
        return 1;                                                                                     /* return error */
    }
    prev |= 1 << 1;                                                                                   /* set fault detection */
    prev |= 1 << 7;                                                                                   /* enable vbias */
    prev &= ~(1 << 6);                                                                                /* set normally off */
    prev |= 1 << 5;                                                                                   /* set shot */
    res = handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);           /* write config */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("max31865: write failed.\n");                                             /* write failed */
       
        return 1;                                                                                     /* return error */
    }
    times = 5000;                                                                                     /* set retry times */
    while (((prev & (1 << 5)) != 0) && (times != 0))                                                  /* check retry times */
    {
        handle->delay_ms(63);                                                                         /* delay 63 ms */
        res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                             /* read config */
        if (res != 0)                                                                                 /* check result */
        {
            handle->debug_print("max31865: read failed.\n");                                          /* read failed */
           
            return 1;                                                                                 /* return error */
        }
        times--;                                                                                      /* retry times-- */
        if (times == 0)                                                                               /* if retry times == 0 */
        {
            handle->debug_print("max31865: read timeout.\n");                                         /* read timeout */
           
            return 1;                                                                                 /* return error */
        }
    }
    memset(buf, 0, sizeof(uint8_t) * 2);                                                              /* clear the buffer */
    res = handle->spi_read(MAX31865_REG_RTD_MSB, (uint8_t *)buf, 2);                                  /* read rtd msb */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                              /* read failed */
       
        return 1;                                                                                     /* return error */
    }
    *raw = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                              /* get raw data */
    if (((*raw) & 0x01) != 0)
    {
        handle->debug_print("max31865: find rtd fault.\n");                                           /* find rtd fault */
       
        return 4;                                                                                     /* return error */
    }
    else
    {
        (*raw) = (*raw) >> 1;                                                                         /* get raw data */
    }
    if (handle->resistor == MAX31865_RESISTOR_100PT)                                                  /* if 100PT */
    {
        resistor = 100.0f;                                                                            /* set resistor type 100 */
    }
    else
    {
        resistor = 1000.0f;                                                                           /* set resistor type 1000 */
    }
    *temp = a_max31865_temperature_conversion((float)(*raw), resistor, handle->ref_resistor);         /* calculate temperature */
    
    return 0;                                                                                         /* success return 0 */
}

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
uint8_t max31865_start_continuous_read(max31865_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    handle->delay_ms(10);                                                                           /* delay 10 ms */
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
       
        return 1;                                                                                   /* return error */
    }
    prev |= 1 << 1;                                                                                 /* set fault detection */
    prev |= 1 << 7;                                                                                 /* enable vbias */
    prev |= 1 << 6;                                                                                 /* set auto mode */
    prev &= ~(1 << 5);                                                                              /* disable shot */

    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_stop_continuous_read(max31865_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    res = handle->spi_read(MAX31865_REG_CONFIG, (uint8_t *)&prev, 1);                               /* read config */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
       
        return 1;                                                                                   /* return error */
    }
    prev |= 1 << 1;                                                                                 /* set fault detection */
    prev &= ~(1 << 7);                                                                              /* disable vbias */
    prev &= ~(1 << 6);                                                                              /* disable auto mode */
    prev &= ~(1 << 5);                                                                              /* disable shot */

    return handle->spi_write(MAX31865_REG_CONFIG | WRITE_ADDRESS_MASK, (uint8_t *)&prev, 1);        /* write config */
}

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
uint8_t max31865_continuous_read(max31865_handle_t *handle, uint16_t *raw, float *temp)
{
    uint8_t res;
    uint8_t buf[2];
    float resistor;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                                            /* clear the buffer */
    res = handle->spi_read(MAX31865_REG_RTD_MSB, (uint8_t *)buf, 2);                                /* read rtd msb */
    if (res != 0)                                                                                   /* check result */
    {
        handle->debug_print("max31865: read failed.\n");                                            /* read failed */
       
        return 1;                                                                                   /* return error */
    }
    *raw = (uint16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                            /* get raw data */
    if (((*raw) & 0x01) != 0)                                                                       /* check error */
    {
        handle->debug_print("max31865: find rtd fault.\n");                                         /* find rtd fault */
       
        return 4;                                                                                   /* return error */
    }
    else
    {
        (*raw) = (*raw) >> 1;                                                                       /* get raw data */
    }
    if (handle->resistor == MAX31865_RESISTOR_100PT)                                                /* choose resistor type */
    {
        resistor = 100.0f;                                                                          /* set resistor type 100 */
    }
    else
    {
        resistor = 1000.0f;                                                                         /* set resistor type 1000 */
    }
    *temp = a_max31865_temperature_conversion((float)(*raw), resistor, handle->ref_resistor);       /* calculate */
    
    return 0;                                                                                       /* success return 0 */
}

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
uint8_t max31865_set_reg(max31865_handle_t *handle, uint8_t reg, uint8_t value)
{
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    return handle->spi_write(reg | WRITE_ADDRESS_MASK, (uint8_t *)&value, 1);        /* write to reg */
}

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
uint8_t max31865_get_reg(max31865_handle_t *handle, uint8_t reg, uint8_t *value)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
    
    return handle->spi_read(reg, (uint8_t *)value, 1);        /* read from reg */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a max31865 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max31865_info(max31865_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(max31865_info_t));                       /* initialize max31865 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
