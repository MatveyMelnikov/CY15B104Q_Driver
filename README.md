# CY15B104Q driver
### Launch
* ```make -f MakefileMain.mk``` - building a production version of the code (does not contain tests);
* ```make TESTS=Y FOR_TARGET=Y -f MakefileMain.mk``` - building a test version for target device;
* ```make TESTS=Y FOR_TARGET=N -f MakefileMain.mk start``` - building test version for host device and running it.

### Hardware
STM32F103C8T6 with UART_3 as debug output.
![cy15b104q](https://github.com/user-attachments/assets/90f440b7-8868-4c9a-9e08-2d685f56572a)
