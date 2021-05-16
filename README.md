<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver MAX31865

[English](/README.md) | [ 简体中文](/README_CN.md)

The MAX31865 is an easy-to-use resistance-to-digital converter optimized for platinum resistance temperature detectors (RTDs). An external resistor sets the sensitivity for the RTD being used and a precision delta-sigma ADC converts the ratio of the RTD resistance to the reference resistance into digital form. The MAX31865’s inputs are protected against overvoltage faults as large as Q45V. Programmable detection of RTD and cable open and short conditions is included. MAX31865 is used in industrial equipment, instruments and medical devices.

LibDriver MAX31865 is the full function driver of MAX31865 launched by LibDriver.It provides functions of PT temperature continuous mode reading, PT temperature single mode reading, etc.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MAX31865 source files.

/interface includes LibDriver MAX31865 SPI platform independent template。

/test includes LibDriver MAX31865 driver test code and this code can test the chip necessary function simply。

/example includes LibDriver MAX31865 sample code.

/doc includes LibDriver MAX31865 offline document.

/datasheet includes MAX31865 datasheet。

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

### Install

Reference /interface SPI platform independent template and finish your platform SPI driver.

Add /src, /interface and /example to your project.

### Usage

#### example basic

```C
uint8_t res;
uint8_t i;
float temp;

res = max31865_basic_init(MAX31865_WIRE_4, MAX31865_RESISTOR_1000PT, 430.f);
if (res)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    res = max31865_basic_read((float *)&temp);
    if (res)
    {
        max31865_basic_deinit();

        return 1;
    }
    max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
    max31865_interface_delay_ms(1000);
    
    ...
    
}

...

max31865_basic_deinit();

return 0;
```

#### example shot

```C
uint8_t res;
uint8_t i;
float temp;

res = max31865_shot_init(MAX31865_WIRE_4, MAX31865_RESISTOR_1000PT, 430.f);
if (res)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    res = max31865_shot_read((float *)&temp);
    if (res)
    {
        max31865_basic_deinit();

        return 1;
    }
    max31865_interface_debug_print("max31865: temperature is %0.4fC.\n", temp);
    max31865_interface_delay_ms(1000);
    
    ...
    
}

...

max31865_shot_deinit();

return 0;
```

### Document

Online documents: https://www.libdriver.com/docs/max31865/index.html

Offline documents: /doc/html/index.html

### Contributing

Please sent an e-mail to lishifenging@outlook.com

### License

Copyright (C) LibDriver 2015-2021 All rights reserved 



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please sent an e-mail to lishifenging@outlook.com