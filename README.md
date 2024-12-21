# HC06 driver
### Launch
* ```make -f MakefileMain.mk``` - building a production version of the code (does not contain tests);
* ```make TESTS=Y FOR_TARGET=Y -f MakefileMain.mk``` - building a test version for target device;
* ```make TESTS=Y FOR_TARGET=N -f MakefileMain.mk start``` - building test version for host device and running it.

### Hardware
STM32F103C8T6 with UART_1 as debug output.
