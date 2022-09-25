### 1. Chip

#### 1.1 Chip Info

chip name : Raspberry Pi 4B.

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(max31865 REQUIRED)
```

### 3. MAX31865

#### 3.1 Command Instruction

​          max31865 is a basic command which can test all max31865 driver function:

​           -i        show max31865 chip and driver information.

​           -h       show max31865 help.

​           -p       show max31865 pin connections of the current board.

​           -t (reg | read <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>)

​           -t  reg        run max31865 register test.

​           -t read <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>        run max31865 read test. times means test times. resistor means reference resistor.

​           -c (read  <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor> | shot <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>)

​           -c read  <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>        run max31865 read function. times means test times. resistor means reference resistor.

​           -c shot  <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>        run max31865 shot function. times means test times. resistor means reference resistor.

#### 3.2 Command Example

```shell
./max31865 -i

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
./max31865 -p

max31865: SCK connected to GPIO11(BCM).
max31865: MISO connected to GPIO9(BCM).
max31865: MOSI connected to GPIO10(BCM).
max31865: CS connected to GPIO8(BCM).
```

```shell
./max31865 -t reg

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
./max31865 -t read 3 -wire 4 -type 100 -r 430.0

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
max31865: continuous read 27.66C.
max31865: continuous read 27.73C.
max31865: continuous read 27.80C.
max31865: single read test.
max31865: single read 27.70C.
max31865: single read 27.73C.
max31865: single read 27.83C.
max31865: finish read test.
```

```shell
./max31865 -c read 3 -wire 4 -type 100 -r 430.0

max31865: 1/3.
max31865: temperature is 27.8326C.
max31865: 2/3.
max31865: temperature is 27.8326C.
max31865: 3/3.
max31865: temperature is 27.8326C.
```

```shell
./max31865 -c shot 3 -wire 4 -type 100 -r 430.0

max31865: 1/3.
max31865: temperature is 27.8326C.
max31865: 2/3.
max31865: temperature is 27.9680C.
max31865: 3/3.
max31865: temperature is 28.0019C.
```

```shell
./max31865 -h

max31865 -i
	show max31865 chip and driver information.
max31865 -h
	show max31865 help.
max31865 -p
	show max31865 pin connections of the current board.
max31865 -t reg
	run max31865 register test.
max31865 -t read <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>
	run max31865 read test.times means test times.resistor means reference resistor.
max31865 -c read <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>
	run max31865 read function.times means test times.resistor means reference resistor.
max31865 -c shot <times> -wire (2 | 3 | 4) -type (100 | 1000) -r <resistor>
	run max31865 shot function.times means test times.resistor means reference resistor.
```

