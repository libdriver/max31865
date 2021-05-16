<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver MAX31865

[English](/README.md) | [ 简体中文](/README_CH.md)

MAX31865是简单易用的热敏电阻至数字输出转换器，优化用于铂电阻温度检测器(RTD)。外部电阻设置RTD灵敏度，高精度Δ- Σ ADC将RTD电阻与基准电阻之比转换为数字输出。MAX31865输入具有高达±45V的过压保护，提供可配置的RTD及电缆开路、短路条件检测。MAX31865被用于工业设备、仪表和医疗装置等领域。

LibDriver MAX31865是LibDriver推出的MAX31865的全功能驱动，该驱动提供PT温度连续模式读取、PT温度单次模式读取等功能。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver MAX31865的源文件。

/interface目录包含了LibDriver MAX31865与平台无关的SPI总线模板。

/test目录包含了LibDriver MAX31865驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver MAX31865编程范例。

/doc目录包含了LibDriver MAX31865离线文档。

/datasheet目录包含了MAX31865数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

### 安装

参考/interface目录下与平台无关的SPI总线模板，完成指定平台的SPI总线驱动。

将/src目录，/interface目录和/example目录加入工程。

### 使用

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

### 文档

在线文档: https://www.libdriver.com/docs/max31865/index.html

离线文档: /doc/html/index.html

### 贡献

请联系lishifenging@outlook.com

### 版权

版权(C) LibDriver 2015-2021 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com