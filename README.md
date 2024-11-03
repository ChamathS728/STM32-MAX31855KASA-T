# STM32-MAX31855KASA-T
Code for the MAX31855KASA+T thermocouple IC, using a Nucleo-G431KB


## How to use

### Setup
An SPI bus should be configured to be running at a bit rate slower than 5MHz. It can work when the bus is running at 500kHz. The following settings are confirmed to work, with the matching SPI bus clock set at 32MHz.

|Basic Parameters||
|------------|--------|
|Frame Format|Motorola|
|Data Size| 8 Bits|
|First Bit| MSB First|

|Clock Parameters||
|------------|--------|
|Prescaler (for Baud Rate)|64|
|Baud Rate| 500.0 KBits/s|
|Clock Polarity (CPOL)|Low|
|Clock Phase (CPHA)| 1 Edge|

|Advanced Parameters||
|------------|--------|
|CRC Calculation|Disabled|
|NSSP Mode|Enabled|
|NSS Signal Type|Software|

The CS pin was done as a GPIO output, which gets used in the MAX31855KASA.c file.

### Using in code
Within the `MAX31855KASA.c` file, the `#include` line should have the include for whichever STM32-based MCU the device is connected to. In this case, the code was written for an STM32G431KB chip, hence it has `#include "stm32g4xx_hal.h"`

Include the header file with `#include "MAX31855KASA.h"`. It is recommended to also drive the CS pin connected to the IC before the main loop to enabled the serial interface.

Define a thermocouple struct instance. An example is done below

```
MAXthermo therm_inst = {
    .hspi= &hspi1,
    .currentTemperature=1.0,
    .rawOutput=1,
    .rawReference=1,
    .rawTemperature=1,
    .errorBits=0
};
```

At this point, we can call `MAXthermo_readThermocouple` to read in 32 bits and get the current temperature as well. An example is shown below

```
MAXthermo_readThermocouple(&therm_inst, &huart2, CS_GPIO_Port, CS_Pin);
```

Note that `&huart2` is a UART handle only for printing to the terminal. By default, this printing within the `readThermocouple` function are disabled. To enable it, define the `VERBOSE` flag within `MAX31855KASA.c` and it'll enable some printing.
