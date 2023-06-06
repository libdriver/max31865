### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MAX31865

#### 3.1 Command Instruction

1. Show max31865 chip and driver information.

   ```shell
   max31865 (-i | --information)
   ```

2. Show max31865 help.

   ```shell
   max31865 (-h | --help)
   ```

3. Show max31865 pin connections of the current board.

   ```shell
   max31865 (-p | --port)
   ```

4. Run max31865 register test.

   ```shell
   max31865 (-t reg | --test=reg)
   ```

5. Run max31865 read test, num means test times, r means reference resistor.

   ```shell
   max31865 (-t read | --test=read) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>] [--resistor=<r>] [--times=<num>]
   ```

6. Run max31865 read function, num means test times, r means reference resistor.

   ```shell
   max31865 (-e read | --example=read) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>] [--resistor=<r>] [--times=<num>]
   ```

7. Run max31865 shot function, num means test times, r means reference resistor.

   ```shell
   max31865 (-e shot | --example=shot) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>] [--resistor=<r>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
max31865 -i

max31865: chip is Maxim Integrated MAX31865.
max31865: manufacturer is Maxim Integrated.
max31865: interface is SPI.
max31865: driver version is 2.0.
max31865: min supply voltage is 3.0V.
max31865: max supply voltage is 3.6V.
max31865: max current is 5.75mA.
max31865: max temperature is 85.0C.
max31865: min temperature is -40.0C.
```

```shell
max31865 -p

max31865: SCK connected to GPIOA PIN5.
max31865: MISO connected to GPIOA PIN6.
max31865: MOSI connected to GPIOA PIN7.
max31865: CS connected to GPIOA PIN4.
```

```shell
max31865 -t reg

max31865: chip is Maxim Integrated MAX31865.
max31865: manufacturer is Maxim Integrated.
max31865: interface is SPI.
max31865: driver version is 2.0.
max31865: min supply voltage is 3.0V.
max31865: max supply voltage is 3.6V.
max31865: max current is 5.75mA.
max31865: max temperature is 85.0C.
max31865: min temperature is -40.0C.
max31865: start register test.
max31865: max31865_set_filter_select/max31865_get_filter_select test.
max31865: set filter select 60Hz.
max31865: check filter select ok.
max31865: set filter select 50Hz.
max31865: check filter select ok.
max31865: max31865_set_wire/max31865_get_wire test.
max31865: set 2 wire.
max31865: check wire ok.
max31865: set 3 wire.
max31865: check wire ok.
max31865: set 4 wire.
max31865: check wire ok.
max31865: max31865_set_resistor/max31865_get_resistor test.
max31865: set 100 pt resistor.
max31865: check resistor ok.
max31865: set 1000 pt resistor.
max31865: check resistor ok.
max31865: max31865_set_reference_resistor/max31865_get_reference_resistor test.
max31865: set reference resistor 411.359985.
max31865: check reference resistor ok.
max31865: max31865_set_vbias/max31865_get_vbias test.
max31865: enable vbias.
max31865: check vbias ok.
max31865: disable vbias.
max31865: check vbias ok.
max31865: max31865_set_fault_detection_cycle_control test.
max31865: check fault detection cycle control ok.
max31865: check fault detection cycle control ok.
max31865: check fault detection cycle control ok.
max31865: check fault detection cycle control ok.
max31865: max31865_get_fault_detection_cycle_control test.
max31865: check get fault detection cycle control ok 0x00.
max31865: max31865_clear_fault_status test.
max31865: check clear fault status ok.
max31865: max31865_set_high_fault_threshold/max31865_get_high_fault_threshold test.
max31865: set high fault threshold 8651.
max31865: check high fault threshold ok.
max31865: max31865_set_low_fault_threshold/max31865_get_low_fault_threshold test.
max31865: set low fault threshold 31561.
max31865: check low fault threshold ok.
max31865: finish register test.
```

```shell
max31865 -t read --wire=4 --type=100 --resistor=430.0 --times=3

max31865: chip is Maxim Integrated MAX31865.
max31865: manufacturer is Maxim Integrated.
max31865: interface is SPI.
max31865: driver version is 2.0.
max31865: min supply voltage is 3.0V.
max31865: max supply voltage is 3.6V.
max31865: max current is 5.75mA.
max31865: max temperature is 85.0C.
max31865: min temperature is -40.0C.
max31865: start read test.
max31865: continuous read test test.
max31865: continuous read 31.12C.
max31865: continuous read 31.19C.
max31865: continuous read 31.22C.
max31865: single read test.
max31865: single read 31.12C.
max31865: single read 31.19C.
max31865: single read 31.22C.
max31865: finish read test.
```

```shell
max31865 -e read --wire=4 --type=100 --resistor=430.0 --times=3

max31865: 1/3.
max31865: temperature is 31.2199C.
max31865: 2/3.
max31865: temperature is 30.9828C.
max31865: 3/3.
max31865: temperature is 31.0505C.
```

```shell
max31865 -e shot --wire=4 --type=100 --resistor=430.0 --times=3

max31865: 1/3.
max31865: temperature is 31.0505C.
max31865: 2/3.
max31865: temperature is 31.0505C.
max31865: 3/3.
max31865: temperature is 31.0823C.
```

```shell
max31865 -h

Usage:
  max31865 (-i | --information)
  max31865 (-h | --help)
  max31865 (-p | --port)
  max31865 (-t reg | --test=reg)
  max31865 (-t read | --test=read) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>]
           [--resistor=<r>] [--times=<num>]
  max31865 (-e read | --example=read) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>]
           [--resistor=<r>] [--times=<num>]
  max31865 (-e shot | --example=shot) [--wire=<2 | 3 | 4>] [--type=<100 | 1000>]
           [--resistor=<r>] [--times=<num>]

Options:
  -e <fifo>, --example=<fifo>    Run the driver example.
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
  -p, --port                     Display the pin connections of the current board.
      --resistor=<r>             Set the reference resistor.([default: 430.0f])
  -t <reg | fifo>, --test=<reg | fifo>
                                 Run the driver test.
      --type=<100 | 1000>        Set the PT resistor type.([default: 100])
      --times=<num>              Set the running times.([default: 3])
      --wire=<2 | 3 | 4>         Set the PT resistor wire.([default: 4])
```

